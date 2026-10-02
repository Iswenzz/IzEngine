#include "XRD3D12Bridge.hpp"

namespace IzEngine
{
	namespace
	{
		// How long a frame's copies may take before their command list is recorded again anyway.
		constexpr DWORD CopyTimeoutMs = 1000;

		template <typename T>
		void SafeRelease(T*& object)
		{
			if (object)
				object->Release();
			object = nullptr;
		}

		// Clicks go through to the window beneath, the game's.
		LRESULT CALLBACK ChildProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if (message == WM_NCHITTEST)
				return HTTRANSPARENT;
			return DefWindowProcA(window, message, wParam, lParam);
		}

		D3D12_RESOURCE_BARRIER Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to)
		{
			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = resource;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = from;
			barrier.Transition.StateAfter = to;
			return barrier;
		}
	}

	DX9XRD3D12Bridge::DX9XRD3D12Bridge(XRD3D12& graphics) : Graphics(graphics) { }

	DX9XRD3D12Bridge::~DX9XRD3D12Bridge()
	{
		Wait();
		for (DX9XRCopy& copy : Copies)
		{
			SafeRelease(copy.List);
			SafeRelease(copy.Allocator);
		}
		SafeRelease(Swapchain);
		if (Child)
			DestroyWindow(Child);
		Child = nullptr;
		SafeRelease(Flush);
		SafeRelease(On12);
		SafeRelease(Fence);
		if (FenceEvent)
			CloseHandle(FenceEvent);
		FenceEvent = nullptr;
	}

	// The renderer's D3D9, made by D3D9On12 on the session's device and queue. Looked up when asked for, as
	// systems before it lack the entry point.
	IDirect3D9* DX9XRD3D12Bridge::CreateDirect3D(XRD3D12& graphics, UINT sdkVersion)
	{
		HMODULE d3d9 = GetModuleHandleA("d3d9.dll");
		if (!d3d9)
			d3d9 = LoadLibraryA("d3d9.dll");
		const auto create =
			d3d9 ? reinterpret_cast<PFN_Direct3DCreate9On12>(GetProcAddress(d3d9, "Direct3DCreate9On12")) : nullptr;
		if (!create || !graphics.Device || !graphics.Queue)
			return nullptr;

		D3D9ON12_ARGS args = {};
		args.Enable9On12 = TRUE;
		args.pD3D12Device = graphics.Device;
		args.ppD3D12Queues[0] = graphics.Queue;
		args.NumQueues = 1;
		return create(sdkVersion, &args, 1);
	}

	// The renderer's device has to be one D3D9On12 made on this session's device, or the copies would read
	// memory of another device.
	bool DX9XRD3D12Bridge::Attach(IDirect3DDevice9* device)
	{
		if (!device || !Graphics.Device)
			return false;

		IDirect3DDevice9On12* on12 = nullptr;
		if (FAILED(device->QueryInterface(__uuidof(IDirect3DDevice9On12), reinterpret_cast<void**>(&on12))))
			return false;

		ID3D12Device* underlying = nullptr;
		const bool same = SUCCEEDED(on12->GetD3D12Device(IID_PPV_ARGS(&underlying))) && underlying == Graphics.Device;
		SafeRelease(underlying);
		if (!same)
		{
			on12->Release();
			return false;
		}

		SafeRelease(On12);
		On12 = on12;
		if (!Fence && FAILED(Graphics.Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence))))
		{
			Fence = nullptr;
			return false;
		}
		if (!FenceEvent)
			FenceEvent = CreateEventA(nullptr, FALSE, FALSE, nullptr);
		return FenceEvent != nullptr;
	}

	// The queries do not survive a reset, and the textures going away may still be read by copies in flight.
	void DX9XRD3D12Bridge::ReleaseCaptures()
	{
		Wait();
		SafeRelease(Flush);
		DX9XRCapture::ReleaseCaptures();
	}

	// Copies this frame's captures into the runtime's images and returns the layers that got theirs.
	bool DX9XRD3D12Bridge::Deliver(const XRLayers& layers, XRLayers& ready)
	{
		ready = layers;
		ready.Eyes = layers.Eyes && Captured[0] && Captured[1];
		ready.Panel = layers.Panel && Captured[2];
		if ((!ready.Eyes && !ready.Panel) || !On12 || !Fence || !D3D9)
			return false;
		if (!Flush && FAILED(D3D9->CreateQuery(D3DQUERYTYPE_EVENT, &Flush)))
			Flush = nullptr;

		struct Transfer
		{
			IDirect3DTexture9* Texture = nullptr;
			XRSwapchain* Swapchain = nullptr;
			ID3D12Resource* Source = nullptr;
			ID3D12Resource* Image = nullptr;
		};
		Transfer transfers[3];
		int count = 0;
		if (ready.Eyes)
		{
			for (int eye = 0; eye < 2; eye++)
				transfers[count++] = { EyeTextures[eye], &OpenXR::EyeSwapchains[eye] };
		}
		if (ready.Panel)
			transfers[count++] = { PanelTexture, &OpenXR::PanelSwapchain };

		DX9XRCopy* copy = NextCopy();
		if (!copy)
			return false;

		// Checked out of D3D9 with the queue told to wait for its drawing, which the flush sends to the GPU.
		for (int i = 0; i < count; i++)
		{
			if (FAILED(On12->UnwrapUnderlyingResource(transfers[i].Texture, Graphics.Queue, IID_PPV_ARGS(&transfers[i].Source))))
				transfers[i].Source = nullptr;
		}
		if (Flush)
		{
			Flush->Issue(D3DISSUE_END);
			Flush->GetData(nullptr, 0, D3DGETDATA_FLUSH);
		}
		for (int i = 0; i < count; i++)
		{
			if (transfers[i].Source)
				transfers[i].Image =
					reinterpret_cast<ID3D12Resource*>(static_cast<uintptr_t>(OpenXR::Acquire(*transfers[i].Swapchain)));
		}

		// An unwrapped texture is in the common state, and the runtime hands its images over as render targets
		// and takes them back the same way.
		D3D12_RESOURCE_BARRIER before[6] = {};
		D3D12_RESOURCE_BARRIER after[6] = {};
		UINT barriers = 0;
		for (int i = 0; i < count; i++)
		{
			const Transfer& transfer = transfers[i];
			if (!transfer.Source || !transfer.Image)
				continue;

			before[barriers] = Transition(transfer.Source, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE);
			after[barriers++] = Transition(transfer.Source, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON);
			before[barriers] = Transition(transfer.Image, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
			after[barriers++] = Transition(transfer.Image, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

		copy->Allocator->Reset();
		copy->List->Reset(copy->Allocator, nullptr);
		if (barriers)
		{
			copy->List->ResourceBarrier(barriers, before);
			for (int i = 0; i < count; i++)
			{
				if (!transfers[i].Source || !transfers[i].Image)
					continue;

				const D3D12_TEXTURE_COPY_LOCATION to = { transfers[i].Image, D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
				const D3D12_TEXTURE_COPY_LOCATION from = { transfers[i].Source, D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
				copy->List->CopyTextureRegion(&to, 0, 0, 0, &from, nullptr);
			}
			copy->List->ResourceBarrier(barriers, after);
		}
		copy->List->Close();

		ID3D12CommandList* lists[] = { copy->List };
		Graphics.Queue->ExecuteCommandLists(1, lists);
		Graphics.Queue->Signal(Fence, ++FenceValue);
		copy->Done = FenceValue;

		// D3D9 waits for the copies before it draws into the textures again.
		for (int i = 0; i < count; i++)
		{
			Transfer& transfer = transfers[i];
			if (transfer.Source)
			{
				UINT64 value = FenceValue;
				On12->ReturnUnderlyingResource(transfer.Texture, 1, &value, &Fence);
				transfer.Source->Release();
			}
			if (transfer.Image)
				OpenXR::Release(*transfer.Swapchain);
		}

		const auto delivered = [&](int i) { return transfers[i].Source && transfers[i].Image; };
		if (ready.Eyes)
			ready.Eyes = delivered(0) && delivered(1);
		if (ready.Panel)
			ready.Panel = delivered(count - 1);
		return ready.Eyes || ready.Panel;
	}

	// D3D9On12 presents a window's back buffer black past a size, so the canvas is shown instead through a
	// swapchain of the session's own device, in a child window over the client area.
	bool DX9XRD3D12Bridge::Present(HWND window)
	{
		RECT client = {};
		if (!window || !CanvasTexture || !On12 || !Fence || !D3D9 || !GetClientRect(window, &client))
			return false;
		const glm::ivec2 size(client.right - client.left, client.bottom - client.top);
		if (size.x <= 0 || size.y <= 0 || size != CanvasSize)
			return false;

		if (!Child)
		{
			WNDCLASSA type = {};
			type.lpfnWndProc = ChildProc;
			type.hInstance = GetModuleHandleA(nullptr);
			type.lpszClassName = "IzEngineXRMirror";
			RegisterClassA(&type);
			Child = CreateWindowExA(0, type.lpszClassName, "", WS_CHILD | WS_VISIBLE, 0, 0, size.x, size.y, window, nullptr,
				type.hInstance, nullptr);
			if (!Child)
				return false;
		}
		if (size != ChildSize)
		{
			SetWindowPos(Child, HWND_TOP, 0, 0, size.x, size.y, SWP_NOACTIVATE);
			if (Swapchain)
			{
				Wait();
				Swapchain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
			}
			ChildSize = size;
		}
		if (!Swapchain)
		{
			IDXGIFactory2* factory = nullptr;
			IDXGIFactory1* base = nullptr;
			if (SUCCEEDED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&base))))
				base->QueryInterface(IID_PPV_ARGS(&factory));
			SafeRelease(base);

			DXGI_SWAP_CHAIN_DESC1 desc = {};
			desc.Width = size.x;
			desc.Height = size.y;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.BufferCount = 2;
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			IDXGISwapChain1* created = nullptr;
			if (factory)
				factory->CreateSwapChainForHwnd(Graphics.Queue, Child, &desc, nullptr, nullptr, &created);
			if (created)
				created->QueryInterface(IID_PPV_ARGS(&Swapchain));
			SafeRelease(created);
			SafeRelease(factory);
			if (!Swapchain)
				return false;
		}

		DX9XRCopy* copy = NextCopy();
		ID3D12Resource* source = nullptr;
		ID3D12Resource* buffer = nullptr;
		if (!copy || FAILED(On12->UnwrapUnderlyingResource(CanvasTexture, Graphics.Queue, IID_PPV_ARGS(&source))))
			return false;
		if (!Flush && FAILED(D3D9->CreateQuery(D3DQUERYTYPE_EVENT, &Flush)))
			Flush = nullptr;
		if (Flush)
		{
			Flush->Issue(D3DISSUE_END);
			Flush->GetData(nullptr, 0, D3DGETDATA_FLUSH);
		}
		Swapchain->GetBuffer(Swapchain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&buffer));

		copy->Allocator->Reset();
		copy->List->Reset(copy->Allocator, nullptr);
		if (buffer)
		{
			const D3D12_RESOURCE_BARRIER before[] = {
				Transition(source, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE),
				Transition(buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST),
			};
			const D3D12_RESOURCE_BARRIER after[] = {
				Transition(source, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON),
				Transition(buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT),
			};
			const D3D12_TEXTURE_COPY_LOCATION to = { buffer, D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
			const D3D12_TEXTURE_COPY_LOCATION from = { source, D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX };
			copy->List->ResourceBarrier(2, before);
			copy->List->CopyTextureRegion(&to, 0, 0, 0, &from, nullptr);
			copy->List->ResourceBarrier(2, after);
		}
		copy->List->Close();

		ID3D12CommandList* lists[] = { copy->List };
		Graphics.Queue->ExecuteCommandLists(1, lists);
		Graphics.Queue->Signal(Fence, ++FenceValue);
		copy->Done = FenceValue;
		UINT64 value = FenceValue;
		On12->ReturnUnderlyingResource(CanvasTexture, 1, &value, &Fence);
		SafeRelease(source);
		if (!buffer)
			return false;

		buffer->Release();
		Swapchain->Present(0, 0);
		return true;
	}

	// A frame's command list is recorded again only once the GPU is done with it, which three frames later
	// it almost always is.
	DX9XRCopy* DX9XRD3D12Bridge::NextCopy()
	{
		DX9XRCopy& copy = Copies[Next];
		Next = (Next + 1) % Copies.size();

		if (!copy.Allocator)
		{
			if (FAILED(Graphics.Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&copy.Allocator))))
			{
				copy.Allocator = nullptr;
				return nullptr;
			}
			if (FAILED(Graphics.Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, copy.Allocator, nullptr,
					IID_PPV_ARGS(&copy.List))))
			{
				copy.List = nullptr;
				SafeRelease(copy.Allocator);
				return nullptr;
			}
			copy.List->Close();
		}
		if (Fence->GetCompletedValue() < copy.Done && SUCCEEDED(Fence->SetEventOnCompletion(copy.Done, FenceEvent)))
			WaitForSingleObject(FenceEvent, CopyTimeoutMs);
		return &copy;
	}

	void DX9XRD3D12Bridge::Wait()
	{
		if (Fence && FenceEvent && Fence->GetCompletedValue() < FenceValue
			&& SUCCEEDED(Fence->SetEventOnCompletion(FenceValue, FenceEvent)))
			WaitForSingleObject(FenceEvent, CopyTimeoutMs);
	}
}

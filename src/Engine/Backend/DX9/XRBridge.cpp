#include "XRBridge.hpp"

namespace IzEngine
{
	namespace
	{
		// How long a readback may take before its frame is dropped rather than waited on further.
		constexpr int ReadbackTimeoutMs = 100;

		template <typename T>
		void SafeRelease(T*& object)
		{
			if (object)
				object->Release();
			object = nullptr;
		}
	}

	DX9XRBridge::DX9XRBridge(XRD3D11& graphics) : Graphics(graphics) { }

	DX9XRBridge::~DX9XRBridge()
	{
		for (DX9XRSlot& slot : Slots)
			ReleaseSlot(slot);
	}

	// The system memory copies survive a reset, but the queries marking them done do not, so a pending
	// frame is dropped.
	void DX9XRBridge::ReleaseCaptures()
	{
		DX9XRCapture::ReleaseCaptures();
		for (DX9XRSlot& slot : Slots)
		{
			SafeRelease(slot.Done);
			slot.Pending = false;
		}
	}

	void DX9XRBridge::Resized()
	{
		for (DX9XRSlot& slot : Slots)
			ReleaseSlot(slot);
	}

	void DX9XRBridge::CreateSlot(DX9XRSlot& slot)
	{
		for (int eye = 0; eye < 2; eye++)
		{
			if (!slot.Eyes[eye])
				D3D9->CreateOffscreenPlainSurface(EyeSize.x, EyeSize.y, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM,
					&slot.Eyes[eye], nullptr);
		}
		if (!slot.Panel)
			D3D9->CreateOffscreenPlainSurface(PanelSize.x, PanelSize.y, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &slot.Panel,
				nullptr);
		if (!slot.Done)
			D3D9->CreateQuery(D3DQUERYTYPE_EVENT, &slot.Done);
	}

	void DX9XRBridge::ReleaseSlot(DX9XRSlot& slot)
	{
		SafeRelease(slot.Eyes[0]);
		SafeRelease(slot.Eyes[1]);
		SafeRelease(slot.Panel);
		SafeRelease(slot.Done);
		slot.Pending = false;
	}

	// This frame starts its way back, and the previous one, already there, is what the runtime gets.
	bool DX9XRBridge::Deliver(const XRLayers& layers, XRLayers& ready)
	{
		Queue(layers);
		return Submit(ready);
	}

	// Starts this frame's copies back to system memory. They are only read on the next frame, by which
	// time the GPU has long finished them, so neither the game nor the runtime waits on the transfer.
	void DX9XRBridge::Queue(const XRLayers& layers)
	{
		if (!D3D9 || !EyeTextures[0])
			return;

		DX9XRSlot& slot = Slots[Current];
		CreateSlot(slot);

		const bool eyes = layers.Eyes && Captured[0] && Captured[1] && slot.Eyes[0] && slot.Eyes[1];
		const bool panel = layers.Panel && Captured[2] && slot.Panel;
		if (!eyes && !panel)
			return;

		bool queued = true;
		if (eyes)
		{
			for (int eye = 0; eye < 2; eye++)
				queued &= SUCCEEDED(D3D9->GetRenderTargetData(EyeSurfaces[eye], slot.Eyes[eye]));
		}
		if (panel)
			queued &= SUCCEEDED(D3D9->GetRenderTargetData(PanelSurface, slot.Panel));
		if (!queued)
			return;

		if (slot.Done)
			slot.Done->Issue(D3DISSUE_END);

		slot.Layers = layers;
		slot.Layers.Eyes = eyes;
		slot.Layers.Panel = panel;
		slot.Pending = true;
		Current ^= 1;
	}

	// Hands the previous frame to the runtime. Returns false when there is nothing to show yet.
	bool DX9XRBridge::Submit(XRLayers& layers)
	{
		DX9XRSlot& slot = Slots[Current];
		if (!slot.Pending || !Graphics.Context)
			return false;
		slot.Pending = false;

		if (slot.Done)
		{
			const ULONGLONG start = GetTickCount64();
			while (slot.Done->GetData(nullptr, 0, D3DGETDATA_FLUSH) == S_FALSE)
			{
				if (GetTickCount64() - start > ReadbackTimeoutMs)
					return false;
				YieldProcessor();
			}
		}

		layers = slot.Layers;
		if (layers.Eyes)
		{
			for (int eye = 0; eye < 2; eye++)
				layers.Eyes &= Upload(OpenXR::EyeSwapchains[eye], slot.Eyes[eye], EyeSize.y);
		}
		if (layers.Panel)
			layers.Panel = Upload(OpenXR::PanelSwapchain, slot.Panel, PanelSize.y);

		Graphics.Context->Flush();
		return layers.Eyes || layers.Panel;
	}

	// BGRA is the byte order of D3DFMT_A8R8G8B8 and copies straight across; RGBA needs its channels swapped.
	bool DX9XRBridge::Upload(XRSwapchain& swapchain, IDirect3DSurface9* surface, int height)
	{
		auto* image = reinterpret_cast<ID3D11Texture2D*>(static_cast<uintptr_t>(OpenXR::Acquire(swapchain)));
		if (!image)
			return false;

		D3DLOCKED_RECT locked = {};
		if (FAILED(surface->LockRect(&locked, nullptr, D3DLOCK_READONLY)))
		{
			OpenXR::Release(swapchain);
			return false;
		}

		if (!OpenXR::Swizzle)
		{
			Graphics.Context->UpdateSubresource(image, 0, nullptr, locked.pBits, locked.Pitch, 0);
		}
		else
		{
			const int width = swapchain.Width;
			Swizzled.resize(static_cast<size_t>(width) * height * 4);

			for (int y = 0; y < height; y++)
			{
				const auto* in = reinterpret_cast<const uint32_t*>(static_cast<const uint8_t*>(locked.pBits) + y * locked.Pitch);
				auto* out = reinterpret_cast<uint32_t*>(Swizzled.data() + static_cast<size_t>(y) * width * 4);

				for (int x = 0; x < width; x++)
				{
					const uint32_t v = in[x];
					out[x] = (v & 0xFF00FF00u) | ((v >> 16) & 0xFFu) | ((v & 0xFFu) << 16);
				}
			}
			Graphics.Context->UpdateSubresource(image, 0, nullptr, Swizzled.data(), width * 4, 0);
		}
		surface->UnlockRect();
		OpenXR::Release(swapchain);
		return true;
	}
}

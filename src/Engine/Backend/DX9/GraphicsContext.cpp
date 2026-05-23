#include "GraphicsContext.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"
#include "Engine/Core/System/Window.hpp"
#include "Engine/Renderer/Base/GPUResource.hpp"

namespace IzEngine
{
	void DX9GraphicsContext::Initialize()
	{
		IZ_ASSERT(Window::Handle, "DX9GraphicsContext needs a valid window handle.");

		if (!Swapped)
		{
			Direct3DCreate9Ex(D3D_SDK_VERSION, &D3DEX);
			D3DEX->QueryInterface(__uuidof(IDirect3D9), reinterpret_cast<void**>(&D3D));

			PresentParameters = { 0 };
			PresentParameters.Windowed = TRUE;
			PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			PresentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
			PresentParameters.BackBufferCount = 1;
			PresentParameters.BackBufferWidth = static_cast<UINT>(Window::Size.x);
			PresentParameters.BackBufferHeight = static_cast<UINT>(Window::Size.y);

			D3DEX->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, reinterpret_cast<HWND>(Window::Handle),
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &PresentParameters, nullptr, &DeviceEx);
			DeviceEx->QueryInterface(__uuidof(IDirect3DDevice9), reinterpret_cast<void**>(&Device));
		}
		StateBlock = CreateScope<DX9StateBlock>();
	}

	void DX9GraphicsContext::Shutdown()
	{
		StateBlock = nullptr;

		if (!Swapped)
		{
			if (Device)
			{
				Device->Release();
				Device = nullptr;
			}
			if (D3D)
			{
				D3D->Release();
				D3D = nullptr;
			}
			if (DeviceEx)
			{
				DeviceEx->Release();
				DeviceEx = nullptr;
			}
			if (D3DEX)
			{
				D3DEX->Release();
				D3DEX = nullptr;
			}
		}
	}

	void DX9GraphicsContext::Reset()
	{
		if (Swapped)
			return;

		HRESULT hr = Device->TestCooperativeLevel();
		if (hr != D3D_OK && hr != D3DERR_DEVICENOTRESET)
			return;

		GPUResource::NotifyBeforeReset();
		ImGui_ImplAPI_InvalidateDeviceObjects();
		hr = Device->Reset(&PresentParameters);

		if (SUCCEEDED(hr))
		{
			GPUResource::NotifyAfterReset();
			ImGui_ImplAPI_CreateDeviceObjects();
		}
	}

	void DX9GraphicsContext::Resize(const vec2& size)
	{
		if (Swapped)
			return;

		PendingResize = true;
		PendingSize = size;
	}

	void DX9GraphicsContext::Present()
	{
		if (Swapped)
			return;

		Device->EndScene();
		HRESULT hr = Device->Present(nullptr, nullptr, nullptr, nullptr);

		if (PendingResize)
		{
			PendingResize = false;
			PresentParameters.BackBufferWidth = static_cast<UINT>(PendingSize.x);
			PresentParameters.BackBufferHeight = static_cast<UINT>(PendingSize.y);
			Reset();
		}
		else if (hr == D3DERR_DEVICELOST)
			Reset();
	}

	void DX9GraphicsContext::Swap(IDirect3D9* d3d, IDirect3DDevice9* device)
	{
		Swapped = true;
		D3D = d3d;
		Device = device;

		IDirect3DSwapChain9* swapChain = nullptr;
		device->GetSwapChain(0, &swapChain);
		swapChain->GetPresentParameters(&PresentParameters);
		swapChain->Release();
	}

	void DX9GraphicsContext::SaveState()
	{
		if (StateBlock)
			StateBlock->Capture();
	}

	void DX9GraphicsContext::RestoreState()
	{
		if (StateBlock)
			StateBlock->Apply();
	}
}

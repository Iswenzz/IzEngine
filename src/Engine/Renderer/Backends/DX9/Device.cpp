#include "Device.hpp"

#include "Core/System/Window.hpp"

namespace IzEngine
{
	void Device::Initialize()
	{
		IZ_ASSERT(Window::Handle, "Device needs a main window.");

		Direct3DCreate9Ex(D3D_SDK_VERSION, &D3DEX);
		D3DEX->QueryInterface(__uuidof(IDirect3D9), reinterpret_cast<void**>(&D3D));

		PresentParameters = { 0 };
		PresentParameters.Windowed = true;
		PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		PresentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
		PresentParameters.BackBufferCount = 1;
		PresentParameters.BackBufferWidth = Window::Size.x;
		PresentParameters.BackBufferHeight = Window::Size.y;

		D3DEX->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, reinterpret_cast<HWND>(Window::Handle),
			D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &PresentParameters, nullptr, &D3DeviceEx);
		D3DeviceEx->QueryInterface(__uuidof(IDirect3DDevice9), reinterpret_cast<void**>(&D3Device));
	}

	void Device::Shutdown()
	{
		if (D3Device)
			D3Device->Release();

		if (D3D)
			D3D->Release();
	}

	void Device::Reset()
	{
		if (Swapped)
			return;

		if (D3Device)
			D3Device->Reset(&PresentParameters);
	}

	void Device::Resize(const vec2& size)
	{
		PresentParameters.BackBufferWidth = size.x;
		PresentParameters.BackBufferHeight = size.y;

		Reset();
	}

	void Device::Begin()
	{
		if (Swapped)
			return;

		D3Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		D3Device->BeginScene();
	}

	void Device::End()
	{
		if (Swapped)
			return;

		D3Device->EndScene();
		D3Device->Present(nullptr, nullptr, nullptr, nullptr);
	}

	void Device::Swap(IDirect3D9* d3d, IDirect3DDevice9* device)
	{
		Swapped = true;

		D3D = d3d;
		D3Device = device;

		IDirect3DSwapChain9* swapChain;
		device->GetSwapChain(0, &swapChain);
		swapChain->GetPresentParameters(&PresentParameters);
	}
}

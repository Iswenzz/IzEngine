#include "Device.hpp"
#include "ImGUI/UI.hpp"

#include "Core/System/Window.hpp"

namespace IzEngine
{
	void Device::Initialize()
	{
		IZ_ASSERT(OSWindow::Handle, "Device needs a main window.");

		Direct3DCreate9Ex(D3D_SDK_VERSION, &D3DEX);
		D3DEX->QueryInterface(__uuidof(IDirect3D9), reinterpret_cast<void**>(&D3D));

		D3DPRESENT_PARAMETERS d3dpp = { 0 };
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = OSWindow::Size.x;
		d3dpp.BackBufferHeight = OSWindow::Size.y;

		D3DEX->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, reinterpret_cast<HWND>(OSWindow::Handle),
			D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, nullptr, &D3DeviceEx);
		D3DeviceEx->QueryInterface(__uuidof(IDirect3DDevice9), reinterpret_cast<void**>(&D3Device));

		CreateScreen();
	}

	void Device::Shutdown()
	{
		if (D3Device)
			D3Device->Release();
		if (D3D)
			D3D->Release();
	}

	void Device::Reset(D3DPRESENT_PARAMETERS* d3dpp)
	{
		if (D3Device)
			D3Device->Reset(d3dpp);
	}

	void Device::CreateScreen()
	{
		D3DVIEWPORT9 viewport;
		D3Device->GetViewport(&viewport);
		D3DDISPLAYMODE displayMode;
		D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		const vec2 position(viewport.X, viewport.Y);
		const vec2 size(viewport.Width, viewport.Height);
		const vec2 display(displayMode.Width, displayMode.Height);

		UI::CreateScreen(position, size, display);
	}

	void Device::Resize(const vec2& size)
	{
		D3DPRESENT_PARAMETERS d3dpp = { 0 };
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferWidth = size.x;
		d3dpp.BackBufferHeight = size.y;

		if (D3Device)
			D3Device->Reset(&d3dpp);
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
		D3D = d3d;
		D3Device = device;
		Swapped = true;

		CreateScreen();
	}
}

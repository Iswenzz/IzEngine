#include "Device.hpp"
#include "ImGUI/UI.hpp"

namespace IW3SR::Engine
{
	void Device::Assign(IDirect3D9* d3d, IDirect3DDevice9* device)
	{
		D3D = d3d;
		D3Device = device;
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

		UI::Get().CreateScreen(position, size, display);
	}
}

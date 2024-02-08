#pragma once
#include "Engine/Backends/DX9.hpp"
#include "Engine/Core/Screen/VirtualScreen.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Graphics device.
	/// </summary>
	class API Device : public IObject
	{
		CLASS_SINGLETON(Device)
	public:
		IDirect3D9* D3D = nullptr;
		IDirect3DDevice9* D3Device = nullptr;
		VirtualScreen Screen;

		/// <summary>
		/// Assign a device.
		/// </summary>
		/// <param name="d3d">The d3d instance.</param>
		/// <param name="device">The device.</param>
		void Assign(IDirect3D9* d3d, IDirect3DDevice9* device);

		/// <summary>
		/// Create the screen.
		/// </summary>
		void CreateScreen();

	private:
		/// <summary>
		/// Initialize the device.
		/// </summary>
		Device() = default;
		virtual ~Device() = default;
	};
}

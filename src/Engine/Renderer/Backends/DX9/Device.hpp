#pragma once
#include "DX9/Base.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Graphics device.
	/// </summary>
	class API Device
	{
		CLASS_SINGLETON(Device)
	public:
		IDirect3D9* D3D = nullptr;
		IDirect3DDevice9* D3Device = nullptr;

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

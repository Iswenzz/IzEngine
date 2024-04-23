#pragma once
#include "DX9/Base.hpp"

namespace IzEngine
{
	/// <summary>
	/// Graphics device.
	/// </summary>
	class API Device
	{
	public:
		static inline IDirect3D9* D3D = nullptr;
		static inline IDirect3DDevice9* D3Device = nullptr;

		/// <summary>
		/// Initialize device.
		/// </summary>
		/// <param name="d3d">The d3d instance.</param>
		/// <param name="device">The device.</param>
		static void Initialize(IDirect3D9* d3d, IDirect3DDevice9* device);

		/// <summary>
		/// Create the screen.
		/// </summary>
		static void CreateScreen();
	};
}

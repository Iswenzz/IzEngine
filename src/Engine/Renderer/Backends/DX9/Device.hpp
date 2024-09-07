#pragma once
#include "DX9/Base.hpp"

#include "Core/Math.hpp"

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

		static inline IDirect3D9Ex* D3DEX = nullptr;
		static inline IDirect3DDevice9Ex* D3DeviceEx = nullptr;

		/// <summary>
		/// Initialize the device.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the device.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Create the screen.
		/// </summary>
		static void CreateScreen();

		/// <summary>
		/// Resize the renderer buffer.
		/// </summary>
		/// <param name="size">The size.</param>
		static void Resize(const vec2& size);

		/// <summary>
		/// Reset the device.
		/// </summary>
		/// <param name="d3dpp">The presentation parameters.</param>
		static void Reset(D3DPRESENT_PARAMETERS* d3dpp);

		/// <summary>
		/// Swap pointers.
		/// </summary>
		/// <param name="d3d">The instance.</param>
		/// <param name="device">The device.</param>
		static void Swap(IDirect3D9* d3d, IDirect3DDevice9* device);

		/// <summary>
		/// Device begin.
		/// </summary>
		static void Begin();

		/// <summary>
		/// Device end.
		/// </summary>
		static void End();

	private:
		static inline bool Swapped = false;
	};
}

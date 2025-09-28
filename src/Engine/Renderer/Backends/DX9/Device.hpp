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
		static inline D3DPRESENT_PARAMETERS PresentParameters;

		/// <summary>
		/// Initialize the device.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the device.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Reset the device.
		/// </summary>
		static void Reset();

		/// <summary>
		/// Resize the renderer buffer.
		/// </summary>
		/// <param name="size">The size.</param>
		static void Resize(const vec2& size);

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

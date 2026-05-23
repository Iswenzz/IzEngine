#pragma once
#include "Base.hpp"
#include "StateBlock.hpp"

namespace IzEngine
{
	class DX9GraphicsContext
	{
	public:
		static inline IDirect3D9* D3D = nullptr;
		static inline IDirect3DDevice9* Device = nullptr;
		static inline D3DPRESENT_PARAMETERS PresentParameters = { 0 };
		static inline bool Swapped = false;

		static void Initialize();
		static void Shutdown();

		static void Reset();
		static void Present();
		static void Swap(IDirect3D9* d3d, IDirect3DDevice9* device);
		static void Resize(const vec2& size);

		static void SaveState();
		static void RestoreState();

	private:
		static inline IDirect3D9Ex* D3DEX = nullptr;
		static inline IDirect3DDevice9Ex* DeviceEx = nullptr;
		static inline Scope<DX9StateBlock> StateBlock = nullptr;
		static inline bool PendingResize;
		static inline vec2 PendingSize;
	};
}

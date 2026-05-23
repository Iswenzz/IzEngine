#pragma once
#include "Base.hpp"
#include "StateBlock.hpp"

#include "Engine/Renderer/Base/GraphicsContext.hpp"

namespace IzEngine
{
	class DX9GraphicsContext : public GraphicsContext
	{
	public:
		static inline IDirect3D9* D3D = nullptr;
		static inline IDirect3DDevice9* Device = nullptr;
		static inline D3DPRESENT_PARAMETERS PresentParameters = { 0 };
		static inline bool Swapped = false;

		DX9GraphicsContext() = default;
		~DX9GraphicsContext() override = default;

		void Initialize() override;
		void Shutdown() override;

		static void Setup();
		static void Destroy();
		static void Reset();
		static void Swap();
		static void Swap(IDirect3D9* d3d, IDirect3DDevice9* device);
		static void Resize(const vec2& size);

		static void SaveState();
		static void RestoreState();

	private:
		static inline Scope<DX9GraphicsContext> Instance = nullptr;
		static inline IDirect3D9Ex* D3DEX = nullptr;
		static inline IDirect3DDevice9Ex* DeviceEx = nullptr;
		static inline DX9StateBlock StateBlock;
		static inline bool PendingResize;
		static inline vec2 PendingSize;
	};
}

#pragma once
#include "Base.hpp"
#include "StateBlock.hpp"

#include "Engine/Backend/OpenXR/D3D11.hpp"
#include "Engine/Backend/OpenXR/OpenXR.hpp"
#include "Engine/Renderer/Base/GPUResource.hpp"

namespace IzEngine
{
	// One frame on its way back from the GPU: the system memory copies of what was captured, and the
	// layers it was drawn for, so it is submitted with the poses that match its pixels.
	struct DX9XRSlot
	{
		IDirect3DSurface9* Eyes[2] = {};
		IDirect3DSurface9* Panel = nullptr;
		IDirect3DQuery9* Done = nullptr;
		bool Pending = false;
		XRLayers Layers;
	};

	// Carries what a D3D9 renderer draws to OpenXR's Direct3D 11 swapchains. A legacy D3D9 device cannot
	// share a texture with D3D11, so the pixels go through system memory, read back a frame late so
	// neither side waits on the other.
	class API DX9XRBridge : public GPUResource
	{
	public:
		DX9XRBridge(XRD3D11& graphics);
		~DX9XRBridge() override;

		bool Prepare(IDirect3DDevice9* device, const glm::ivec2& eyeSize, const glm::ivec2& panelSize);
		void CaptureEye(int eye, IDirect3DSurface9* source);
		void CapturePanel(IDirect3DSurface9* source, const RECT& area);
		void Queue(const XRLayers& layers);
		bool Submit(XRLayers& layers);
		void Mirror(IDirect3DSurface9* screen, bool eyes, bool panel, const vec2& focus);
		void Mirror(IDirect3DSurface9* screen, IDirect3DSurface9* source);
		IDirect3DSurface9* RenderTarget(IDirect3DDevice9* device, const glm::ivec2& size, D3DMULTISAMPLE_TYPE samples,
			DWORD quality);

		void Release() override;

	private:
		XRD3D11& Graphics;
		IDirect3DDevice9* D3D9 = nullptr;
		IDirect3DTexture9* EyeTextures[2] = {};
		IDirect3DSurface9* EyeSurfaces[2] = {};
		IDirect3DTexture9* PanelTexture = nullptr;
		IDirect3DSurface9* PanelSurface = nullptr;
		IDirect3DSurface9* Target = nullptr;
		DX9StateBlock State;
		DX9XRSlot Slots[2];
		int Current = 0;
		glm::ivec2 EyeSize{};
		glm::ivec2 PanelSize{};
		bool Captured[3] = {};
		std::vector<uint8_t> Swizzled;

		void ReleaseCaptures();
		void CreateSlot(DX9XRSlot& slot);
		void ReleaseSlot(DX9XRSlot& slot);
		bool Upload(XRSwapchain& swapchain, IDirect3DSurface9* surface, int height);
		RECT Fit(IDirect3DSurface9* screen, const glm::ivec2& size) const;
		RECT Crop(IDirect3DSurface9* screen, const vec2& focus) const;
		void DrawQuad(IDirect3DTexture9* texture, IDirect3DSurface9* target, const RECT& rect);
	};
}

#pragma once
#include "Base.hpp"
#include "StateBlock.hpp"

#include "Engine/Backend/OpenXR/OpenXR.hpp"
#include "Engine/Renderer/Base/GPUResource.hpp"

namespace IzEngine
{
	// The D3D9 side of carrying a renderer's frames to OpenXR: the eyes and the panel captured into textures
	// of their own, the frame buffer the renderer draws into at the headset's size, and the window's mirror.
	// How the captures reach the runtime is left to the bridge built on it.
	class API DX9XRCapture : public GPUResource
	{
	public:
		DX9XRCapture();
		~DX9XRCapture() override;

		bool Prepare(IDirect3DDevice9* device, const glm::ivec2& eyeSize, const glm::ivec2& panelSize);
		void CaptureEye(int eye, IDirect3DSurface9* source);
		void CapturePanel(IDirect3DSurface9* source, const RECT& area);
		void Mirror(IDirect3DSurface9* screen, bool eyes, bool panel, const vec2& focus);
		void Mirror(IDirect3DSurface9* screen, IDirect3DSurface9* source);
		IDirect3DSurface9* RenderTarget(IDirect3DDevice9* device, const glm::ivec2& size, D3DMULTISAMPLE_TYPE samples,
			DWORD quality);
		IDirect3DSurface9* Canvas(IDirect3DDevice9* device, const glm::ivec2& size);

		virtual bool Attach(IDirect3DDevice9* device);
		virtual bool Deliver(const XRLayers& layers, XRLayers& ready) = 0;

		void Release() override;

	protected:
		IDirect3DDevice9* D3D9 = nullptr;
		IDirect3DTexture9* EyeTextures[2] = {};
		IDirect3DSurface9* EyeSurfaces[2] = {};
		IDirect3DTexture9* PanelTexture = nullptr;
		IDirect3DSurface9* PanelSurface = nullptr;
		glm::ivec2 EyeSize{};
		glm::ivec2 PanelSize{};
		bool Captured[3] = {};

		virtual void ReleaseCaptures();
		virtual void Resized();

	private:
		IDirect3DSurface9* Target = nullptr;
		IDirect3DSurface9* CanvasSurface = nullptr;
		glm::ivec2 CanvasSize{};
		DX9StateBlock State;

		RECT Fit(IDirect3DSurface9* screen, const glm::ivec2& size) const;
		RECT Crop(IDirect3DSurface9* screen, const vec2& focus) const;
		void DrawQuad(IDirect3DTexture9* texture, IDirect3DSurface9* target, const RECT& rect);
	};
}

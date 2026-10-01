#include "XRCapture.hpp"

namespace IzEngine
{
	namespace
	{
		struct QuadVertex
		{
			float X, Y, Z, Rhw;
			float U, V;
		};

		template <typename T>
		void SafeRelease(T*& object)
		{
			if (object)
				object->Release();
			object = nullptr;
		}
	}

	DX9XRCapture::DX9XRCapture()
	{
		GPUResource::RegisterResource(this);
	}

	DX9XRCapture::~DX9XRCapture()
	{
		GPUResource::UnregisterResource(this);
		DX9XRCapture::ReleaseCaptures();
		SafeRelease(Target);
	}

	// Everything in D3DPOOL_DEFAULT, which the device has to be rid of before a reset. The renderer has
	// let go of the stand-in back buffer by then, so our reference is the last.
	void DX9XRCapture::Release()
	{
		ReleaseCaptures();
		SafeRelease(Target);
	}

	void DX9XRCapture::ReleaseCaptures()
	{
		for (int eye = 0; eye < 2; eye++)
		{
			SafeRelease(EyeSurfaces[eye]);
			SafeRelease(EyeTextures[eye]);
		}
		SafeRelease(PanelSurface);
		SafeRelease(PanelTexture);
		EyeSize = {};
		PanelSize = {};
	}

	void DX9XRCapture::Resized() { }

	// Whether the bridge can carry frames from this device.
	bool DX9XRCapture::Attach(IDirect3DDevice9* device)
	{
		return device != nullptr;
	}

	bool DX9XRCapture::Prepare(IDirect3DDevice9* device, const glm::ivec2& eyeSize, const glm::ivec2& panelSize)
	{
		if (!device || eyeSize.x <= 0 || eyeSize.y <= 0 || panelSize.x <= 0 || panelSize.y <= 0)
			return false;

		if (device != D3D9 || eyeSize != EyeSize || panelSize != PanelSize)
		{
			ReleaseCaptures();
			Resized();
			D3D9 = device;
		}
		Captured[0] = Captured[1] = Captured[2] = false;

		if (EyeTextures[0] && EyeTextures[1] && PanelTexture)
			return true;

		bool created = true;
		for (int eye = 0; eye < 2; eye++)
		{
			created &= SUCCEEDED(D3D9->CreateTexture(eyeSize.x, eyeSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, &EyeTextures[eye], nullptr));
			created &= EyeTextures[eye] && SUCCEEDED(EyeTextures[eye]->GetSurfaceLevel(0, &EyeSurfaces[eye]));
		}
		created &= SUCCEEDED(D3D9->CreateTexture(panelSize.x, panelSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, &PanelTexture, nullptr));
		created &= PanelTexture && SUCCEEDED(PanelTexture->GetSurfaceLevel(0, &PanelSurface));

		if (!created)
		{
			ReleaseCaptures();
			return false;
		}
		EyeSize = eyeSize;
		PanelSize = panelSize;
		return true;
	}

	void DX9XRCapture::CaptureEye(int eye, IDirect3DSurface9* source)
	{
		if (!source || !EyeSurfaces[eye])
			return;

		D3DSURFACE_DESC desc = {};
		source->GetDesc(&desc);
		const bool scaled = static_cast<int>(desc.Width) != EyeSize.x || static_cast<int>(desc.Height) != EyeSize.y;

		Captured[eye] = SUCCEEDED(
			D3D9->StretchRect(source, nullptr, EyeSurfaces[eye], nullptr, scaled ? D3DTEXF_LINEAR : D3DTEXF_NONE));
	}

	void DX9XRCapture::CapturePanel(IDirect3DSurface9* source, const RECT& area)
	{
		if (!source || !PanelSurface)
			return;

		Captured[2] = SUCCEEDED(D3D9->StretchRect(source, &area, PanelSurface, nullptr, D3DTEXF_LINEAR));
	}

	// The window's own back buffer: the left eye cropped to fill it around where the eye looks, and the
	// panel over it at its own proportions, or the panel alone when no world was drawn.
	void DX9XRCapture::Mirror(IDirect3DSurface9* screen, bool eyes, bool panel, const vec2& focus)
	{
		if (!screen || !D3D9 || EyeSize.x <= 0 || EyeSize.y <= 0)
			return;

		D3D9->ColorFill(screen, nullptr, D3DCOLOR_XRGB(0, 0, 0));
		if (eyes && Captured[0] && EyeSurfaces[0])
		{
			const RECT source = Crop(screen, focus);
			D3D9->StretchRect(EyeSurfaces[0], &source, screen, nullptr, D3DTEXF_LINEAR);
		}
		if (panel && Captured[2] && PanelTexture)
			DrawQuad(PanelTexture, screen, Fit(screen, PanelSize));
	}

	// Whatever the renderer drew into a surface of the eye's size, kept to its proportions.
	void DX9XRCapture::Mirror(IDirect3DSurface9* screen, IDirect3DSurface9* source)
	{
		if (!screen || !source || !D3D9 || EyeSize.x <= 0 || EyeSize.y <= 0)
			return;

		D3D9->ColorFill(screen, nullptr, D3DCOLOR_XRGB(0, 0, 0));
		const RECT rect = Fit(screen, EyeSize);
		D3D9->StretchRect(source, nullptr, screen, &rect, D3DTEXF_LINEAR);
	}

	// Where an image of this size goes in the window, kept to its proportions and centred.
	RECT DX9XRCapture::Fit(IDirect3DSurface9* screen, const glm::ivec2& size) const
	{
		D3DSURFACE_DESC desc = {};
		screen->GetDesc(&desc);

		const float scale = std::min(static_cast<float>(desc.Width) / size.x, static_cast<float>(desc.Height) / size.y);
		const LONG width = static_cast<LONG>(size.x * scale);
		const LONG height = static_cast<LONG>(size.y * scale);
		const LONG left = (static_cast<LONG>(desc.Width) - width) / 2;
		const LONG top = (static_cast<LONG>(desc.Height) - height) / 2;
		return { left, top, left + width, top + height };
	}

	// The largest part of the eye shaped like the window, centred on the focus as far as the eye reaches.
	RECT DX9XRCapture::Crop(IDirect3DSurface9* screen, const vec2& focus) const
	{
		D3DSURFACE_DESC desc = {};
		screen->GetDesc(&desc);
		if (!desc.Width || !desc.Height)
			return { 0, 0, EyeSize.x, EyeSize.y };

		const float aspect = static_cast<float>(desc.Width) / static_cast<float>(desc.Height);
		const float width = std::min(static_cast<float>(EyeSize.x), EyeSize.y * aspect);
		const float height = width / aspect;
		const float left = std::clamp(focus.x * EyeSize.x - width * 0.5f, 0.0f, EyeSize.x - width);
		const float top = std::clamp(focus.y * EyeSize.y - height * 0.5f, 0.0f, EyeSize.y - height);
		return { std::lround(left), std::lround(top), std::lround(left + width), std::lround(top + height) };
	}

	// A render target for the renderer to draw into in place of its back buffer, so everything it sizes
	// to the display is drawn at the headset's size while the window keeps its own. The caller takes a
	// reference, as it would from GetBackBuffer.
	IDirect3DSurface9* DX9XRCapture::RenderTarget(IDirect3DDevice9* device, const glm::ivec2& size,
		D3DMULTISAMPLE_TYPE samples, DWORD quality)
	{
		if (!Target
			&& FAILED(device->CreateRenderTarget(size.x, size.y, D3DFMT_A8R8G8B8, samples, quality, FALSE, &Target, nullptr)))
			Target = nullptr;

		if (Target)
			Target->AddRef();
		return Target;
	}

	// Draws through the fixed function pipeline, bracketed by a state block so the device comes back
	// exactly as the renderer's own state cache believes it to be. The texture is premultiplied, hence
	// ONE rather than SRCALPHA.
	void DX9XRCapture::DrawQuad(IDirect3DTexture9* texture, IDirect3DSurface9* target, const RECT& rect)
	{
		if (!texture || !target)
			return;

		// The depth buffer is the renderer's, at its own size and sample count, which a target of another
		// size or count cannot be drawn with.
		IDirect3DSurface9* previous = nullptr;
		IDirect3DSurface9* depth = nullptr;
		D3D9->GetRenderTarget(0, &previous);
		D3D9->GetDepthStencilSurface(&depth);
		State.Capture();

		D3D9->SetRenderTarget(0, target);
		D3D9->SetDepthStencilSurface(nullptr);

		D3DSURFACE_DESC desc = {};
		target->GetDesc(&desc);
		const D3DVIEWPORT9 viewport = { 0, 0, desc.Width, desc.Height, 0.0f, 1.0f };
		D3D9->SetViewport(&viewport);

		D3D9->SetVertexShader(nullptr);
		D3D9->SetPixelShader(nullptr);
		D3D9->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
		D3D9->SetTexture(0, texture);
		D3D9->SetTexture(1, nullptr);

		D3D9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		D3D9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		D3D9->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		D3D9->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		D3D9->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		D3D9->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		D3D9->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		D3D9->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		D3D9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		D3D9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		D3D9->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		D3D9->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		D3D9->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		D3D9->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, FALSE);

		D3D9->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		D3D9->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		D3D9->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		D3D9->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		D3D9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		D3D9->SetRenderState(D3DRS_LIGHTING, FALSE);
		D3D9->SetRenderState(D3DRS_FOGENABLE, FALSE);
		D3D9->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3D9->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
		D3D9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		D3D9->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		D3D9->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);

		D3D9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		D3D9->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
		D3D9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		D3D9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		D3D9->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		const float left = static_cast<float>(rect.left) - 0.5f;
		const float top = static_cast<float>(rect.top) - 0.5f;
		const float right = static_cast<float>(rect.right) - 0.5f;
		const float bottom = static_cast<float>(rect.bottom) - 0.5f;

		const QuadVertex quad[4] = {
			{ left, top, 0.0f, 1.0f, 0.0f, 0.0f },
			{ right, top, 0.0f, 1.0f, 1.0f, 0.0f },
			{ left, bottom, 0.0f, 1.0f, 0.0f, 1.0f },
			{ right, bottom, 0.0f, 1.0f, 1.0f, 1.0f },
		};
		D3D9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(QuadVertex));

		D3D9->SetRenderTarget(0, previous);
		D3D9->SetDepthStencilSurface(depth);
		State.Apply();
		SafeRelease(previous);
		SafeRelease(depth);
	}
}

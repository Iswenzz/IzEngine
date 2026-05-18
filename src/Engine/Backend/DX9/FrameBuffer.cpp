#include "FrameBuffer.hpp"
#include "GraphicsContext.hpp"

namespace IzEngine
{
	static D3DFORMAT FrameBufferTextureFormatToDX9(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::RGBA8:
			return D3DFMT_A8R8G8B8;
		case FrameBufferTextureFormat::RED_INTEGER:
			return D3DFMT_R32F;
		case FrameBufferTextureFormat::DEPTH24STENCIL8:
			return D3DFMT_D24S8;
		}
		IZ_ASSERT(false, "Unknown FrameBufferTextureFormat.");
		return D3DFMT_UNKNOWN;
	}

	DX9FrameBuffer::DX9FrameBuffer(const FrameBufferSpecification& spec) : Specification(spec)
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9FrameBuffer requires an initialized device.");
		Invalidate();
	}

	DX9FrameBuffer::~DX9FrameBuffer()
	{
		Release();
	}

	void DX9FrameBuffer::Invalidate()
	{
		Release();

		for (const auto& attachment : Specification.Attachments.Attachments)
		{
			if (attachment.Format == FrameBufferTextureFormat::DEPTH24STENCIL8)
			{
				DX9GraphicsContext::Device->CreateDepthStencilSurface(Specification.Width, Specification.Height,
					D3DFMT_D24S8, Specification.Samples > 1 ? D3DMULTISAMPLE_NONMASKABLE : D3DMULTISAMPLE_NONE, 0, TRUE,
					&DepthStencilSurface, nullptr);
			}
			else
			{
				IDirect3DTexture9* texture = nullptr;
				DX9GraphicsContext::Device->CreateTexture(Specification.Width, Specification.Height, 1,
					D3DUSAGE_RENDERTARGET, FrameBufferTextureFormatToDX9(attachment.Format), D3DPOOL_DEFAULT, &texture,
					nullptr);

				IDirect3DSurface9* surface = nullptr;
				texture->GetSurfaceLevel(0, &surface);

				ColorAttachments.push_back(texture);
				ColorSurfaces.push_back(surface);
			}
		}
	}

	void DX9FrameBuffer::Release()
	{
		for (auto& surface : ColorSurfaces)
		{
			if (surface)
			{
				surface->Release();
				surface = nullptr;
			}
		}
		for (auto& texture : ColorAttachments)
		{
			if (texture)
			{
				texture->Release();
				texture = nullptr;
			}
		}
		if (DepthStencilSurface)
		{
			DepthStencilSurface->Release();
			DepthStencilSurface = nullptr;
		}
		ColorSurfaces.clear();
		ColorAttachments.clear();
	}

	void DX9FrameBuffer::Bind()
	{
		IZ_ASSERT(!ColorSurfaces.empty(), "FrameBuffer has no color attachments.");

		DX9GraphicsContext::Device->GetRenderTarget(0, &PreviousRenderTarget);
		DX9GraphicsContext::Device->GetDepthStencilSurface(&PreviousDepthStencil);

		for (uint32_t i = 0; i < ColorSurfaces.size(); i++)
			DX9GraphicsContext::Device->SetRenderTarget(i, ColorSurfaces[i]);

		if (DepthStencilSurface)
			DX9GraphicsContext::Device->SetDepthStencilSurface(DepthStencilSurface);
	}

	void DX9FrameBuffer::Unbind()
	{
		DX9GraphicsContext::Device->SetRenderTarget(0, PreviousRenderTarget);
		DX9GraphicsContext::Device->SetDepthStencilSurface(PreviousDepthStencil);

		if (PreviousRenderTarget)
		{
			PreviousRenderTarget->Release();
			PreviousRenderTarget = nullptr;
		}
		if (PreviousDepthStencil)
		{
			PreviousDepthStencil->Release();
			PreviousDepthStencil = nullptr;
		}
	}

	void DX9FrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		Specification.Width = width;
		Specification.Height = height;
		Invalidate();
	}

	int DX9FrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		IZ_ASSERT(attachmentIndex < ColorAttachments.size(), "Attachment index out of range.");

		IDirect3DSurface9* offscreen = nullptr;
		DX9GraphicsContext::Device->CreateOffscreenPlainSurface(Specification.Width, Specification.Height,
			D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &offscreen, nullptr);

		DX9GraphicsContext::Device->GetRenderTargetData(ColorSurfaces[attachmentIndex], offscreen);

		D3DLOCKED_RECT locked;
		offscreen->LockRect(&locked, nullptr, D3DLOCK_READONLY);

		int* data = static_cast<int*>(locked.pBits);
		int pixel = data[y * (locked.Pitch / sizeof(int)) + x];

		offscreen->UnlockRect();
		offscreen->Release();

		return pixel;
	}

	void DX9FrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		IZ_ASSERT(attachmentIndex < ColorSurfaces.size(), "Attachment index out of range.");

		DX9GraphicsContext::Device->SetRenderTarget(0, ColorSurfaces[attachmentIndex]);
		DX9GraphicsContext::Device->Clear(0, nullptr, D3DCLEAR_TARGET, static_cast<D3DCOLOR>(value), 1.0f, 0);
	}

	uint32_t DX9FrameBuffer::GetColorAttachmentID(uint32_t index) const
	{
		IZ_ASSERT(index < ColorAttachments.size(), "Attachment index out of range.");
		return reinterpret_cast<uint32_t>(ColorAttachments[index]);
	}

	const FrameBufferSpecification& DX9FrameBuffer::GetSpecification() const
	{
		return Specification;
	}
}

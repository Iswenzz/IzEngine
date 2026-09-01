#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Base/FrameBuffer.hpp"

namespace IzEngine
{
	class DX9FrameBuffer : public FrameBuffer
	{
	public:
		DX9FrameBuffer(const FrameBufferSpecification& spec);
		~DX9FrameBuffer() override;

		void Bind() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;
		int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		void ClearAttachment(uint32_t attachmentIndex, int value) override;
		uintptr_t GetColorAttachmentID(uint32_t index = 0) const override;
		const FrameBufferSpecification& GetSpecification() const override;

	private:
		FrameBufferSpecification Specification;
		IDirect3DSurface9* DepthStencilSurface = nullptr;
		IDirect3DSurface9* PreviousRenderTarget = nullptr;
		IDirect3DSurface9* PreviousDepthStencil = nullptr;
		std::vector<IDirect3DTexture9*> ColorAttachments;
		std::vector<IDirect3DSurface9*> ColorSurfaces;

		void Invalidate();
		void Release();
	};
}

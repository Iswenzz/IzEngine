#include "FrameBuffer.hpp"
#include "RendererAPI.hpp"

#include "Engine/Backend/DX9/FrameBuffer.hpp"

namespace IzEngine
{
	FrameBufferTextureSpecification::FrameBufferTextureSpecification(FrameBufferTextureFormat format) : Format(format)
	{
	}

	FrameBufferAttachmentSpecification::FrameBufferAttachmentSpecification(
		std::initializer_list<FrameBufferTextureSpecification> attachments)
		: Attachments(attachments)
	{
	}

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateRef<DX9FrameBuffer>(spec);
		}
		return nullptr;
	}
}

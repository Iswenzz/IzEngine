#include "UniformBuffer.hpp"
#include "RendererAPI.hpp"

#include "Engine/Backend/DX9/UniformBuffer.hpp"

namespace IzEngine
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateRef<DX9UniformBuffer>(size, binding);
		}
		return nullptr;
	}
}

#include "VertexArray.hpp"
#include "RendererAPI.hpp"

#include "Engine/Backend/DX9/VertexArray.hpp"

namespace IzEngine
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateRef<DX9VertexArray>();
		}
		return nullptr;
	}
}

#include "GraphicsContext.hpp"
#include "RendererAPI.hpp"

#include "Engine/Backend/DX9/GraphicsContext.hpp"

namespace IzEngine
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateScope<DX9GraphicsContext>(window);
		}
		return nullptr;
	}
}

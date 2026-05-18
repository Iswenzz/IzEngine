#include "RendererAPI.hpp"
#include "RenderCommand.hpp"

#include "Engine/Backend/DX9/RendererAPI.hpp"

namespace IzEngine
{
	Scope<RendererAPI> RendererAPI::Create(RendererBackend api)
	{
		Backend = api;
		switch (Backend)
		{
		case RendererBackend::DX9:
			return CreateScope<DX9RendererAPI>();
		}
		IZ_ASSERT(false, "Unknown backend.");
		exit(-1);
		return nullptr;
	}

	RendererBackend RendererAPI::GetAPI()
	{
		return Backend;
	}

	std::string RendererAPI::GetShaderPath(const std::string& name)
	{
		switch (Backend)
		{
		case RendererBackend::DX9:
			return std::format("Shaders/DX9/{}.hlsl", name);
		}
		IZ_ASSERT(false, "Unknown backend.");
		return "";
	}
}

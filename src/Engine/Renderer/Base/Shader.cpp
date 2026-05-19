#include "Shader.hpp"

#include "Engine/Backend/DX9/Shader.hpp"
#include "Engine/Renderer/Base/RendererAPI.hpp"

namespace IzEngine
{
	Ref<Shader> Shader::Create(const ShaderSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return DX9Shader::Create(spec);
		}
		return nullptr;
	}
}

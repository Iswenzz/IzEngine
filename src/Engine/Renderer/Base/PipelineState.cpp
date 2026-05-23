#include "PipelineState.hpp"

#include "Engine/Backend/DX9/PipelineState.hpp"
#include "Engine/Renderer/Base/RendererAPI.hpp"

namespace IzEngine
{
	const PipelineStateSpecification& PipelineState::GetSpecification() const
	{
		return Spec;
	}

	Ref<PipelineState> PipelineState::Create(const PipelineStateSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateRef<DX9PipelineState>(spec);
		}
		IZ_ASSERT(false, "Unknown backend.");
		exit(-1);
	}

	Ref<PipelineState> PipelineState::Default2D()
	{
		return Create({ .DepthTest = false,
			.DepthWrite = false,
			.Fog = false,
			.Stencil = false,
			.Scissor = false,
			.Cull = CullMode::None,
			.Blend = BlendMode::Alpha,
			.Fill = FillMode::Solid });
	}

	Ref<PipelineState> PipelineState::Default3D()
	{
		return Create({ .DepthTest = true,
			.DepthWrite = true,
			.Fog = false,
			.Stencil = false,
			.Scissor = false,
			.Cull = CullMode::Back,
			.Blend = BlendMode::None,
			.Fill = FillMode::Solid });
	}
}

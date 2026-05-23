#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	enum class CullMode
	{
		None,
		Front,
		Back
	};

	enum class BlendMode
	{
		None,
		Alpha,
		Additive,
		Multiply
	};

	enum class FillMode
	{
		Solid,
		Wireframe
	};

	struct PipelineStateSpecification
	{
		bool DepthTest = true;
		bool DepthWrite = true;
		bool Fog = false;
		bool Stencil = false;
		bool Scissor = false;
		CullMode Cull = CullMode::Back;
		BlendMode Blend = BlendMode::None;
		FillMode Fill = FillMode::Solid;
	};

	class PipelineState
	{
	public:
		virtual ~PipelineState() = default;

		virtual void Bind() = 0;

		const PipelineStateSpecification& GetSpecification() const;

		static Ref<PipelineState> Create(const PipelineStateSpecification& spec);

		static Ref<PipelineState> Default2D();
		static Ref<PipelineState> Default3D();

	protected:
		PipelineStateSpecification Spec;
	};
}

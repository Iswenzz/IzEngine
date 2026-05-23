#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Base/PipelineState.hpp"

namespace IzEngine
{
	class DX9PipelineState : public PipelineState
	{
	public:
		DX9PipelineState(const PipelineStateSpecification& spec);
		~DX9PipelineState() override = default;

		void Bind() override;
	};
}

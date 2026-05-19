#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Base/UniformBuffer.hpp"

namespace IzEngine
{
	class DX9UniformBuffer : public UniformBuffer
	{
	public:
		DX9UniformBuffer(uint32_t size, uint32_t binding);
		~DX9UniformBuffer() override;

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		std::vector<float> Data;
		uint32_t Binding = 0;
		uint32_t Size = 0;
	};
}

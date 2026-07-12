#include "UniformBuffer.hpp"
#include "GraphicsContext.hpp"

namespace IzEngine
{
	DX9UniformBuffer::DX9UniformBuffer(uint32_t size, uint32_t binding) : Binding(binding), Size(size)
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9UniformBuffer requires an initialized device.");
		IZ_ASSERT(size % 4 == 0, "UniformBuffer size must be a multiple of 4 bytes.");

		// DX9 constants are uploaded as float4 registers, allocate enough float4s to hold the requested size
		uint32_t float4Count = (size + 15) / 16;
		Data.resize(float4Count * 4, 0.0f);
	}

	DX9UniformBuffer::~DX9UniformBuffer() { }

	void DX9UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		IZ_ASSERT(data, "Data is null.");
		IZ_ASSERT(offset + size <= Size, "Data exceeds buffer size.");

		memcpy(reinterpret_cast<uint8_t*>(Data.data()) + offset, data, size);

		uint32_t float4Count = (Data.size() * sizeof(float) + 15) / 16;
		uint32_t startRegister = Binding;

		// Upload to both vertex and pixel shader constant registers
		DX9GraphicsContext::Device->SetVertexShaderConstantF(startRegister, Data.data(), float4Count);
		DX9GraphicsContext::Device->SetPixelShaderConstantF(startRegister, Data.data(), float4Count);
	}
}

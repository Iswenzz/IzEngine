#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Base/Buffer.hpp"

namespace IzEngine
{
	class DX9VertexBuffer : public VertexBuffer
	{
	public:
		DX9VertexBuffer(uint32_t size);
		DX9VertexBuffer(float* vertices, uint32_t size);
		~DX9VertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;
		void Release() override;
		void OnBeforeReset() override;
		void OnAfterReset() override;

		void SetData(const void* data, uint32_t size) override;
		const BufferLayout& GetLayout() const override;
		void SetLayout(const BufferLayout& layout) override;

	private:
		IDirect3DVertexBuffer9* VertexBuffer = nullptr;
		BufferLayout Layout;
		uint32_t Size = 0;
	};

	class DX9IndexBuffer : public IndexBuffer
	{
	public:
		DX9IndexBuffer(uint32_t* indices, uint32_t count);
		~DX9IndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;
		void Release() override;
		void OnBeforeReset() override;
		void OnAfterReset() override;

		uint32_t GetCount() const override;

	private:
		IDirect3DIndexBuffer9* IndexBuffer = nullptr;
		std::vector<uint32_t> Indices;
		uint32_t Count = 0;
	};
}

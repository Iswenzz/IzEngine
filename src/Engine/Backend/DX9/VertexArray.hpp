#pragma once
#include "Base.hpp"

#include "Engine/Renderer/Base/VertexArray.hpp"

namespace IzEngine
{
	class DX9VertexArray : public VertexArray
	{
	public:
		DX9VertexArray();
		~DX9VertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
		const Ref<IndexBuffer>& GetIndexBuffer() const override;

	private:
		IDirect3DVertexDeclaration9* VertexDeclaration = nullptr;
		std::vector<Ref<VertexBuffer>> Vertices;
		Ref<IndexBuffer> Indices;
	};
}

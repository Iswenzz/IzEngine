#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "GraphicsContext.hpp"

namespace IzEngine
{
	static D3DDECLTYPE GetType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
			return D3DDECLTYPE_FLOAT1;
		case ShaderDataType::Float2:
			return D3DDECLTYPE_FLOAT2;
		case ShaderDataType::Float3:
			return D3DDECLTYPE_FLOAT3;
		case ShaderDataType::Float4:
			return D3DDECLTYPE_FLOAT4;
		case ShaderDataType::Int:
			return D3DDECLTYPE_UBYTE4;
		case ShaderDataType::Bool:
			return D3DDECLTYPE_UBYTE4;
		}
		IZ_ASSERT(false, "Unsupported ShaderDataType for DX9.");
		return D3DDECLTYPE_UNUSED;
	}

	static D3DDECLUSAGE GetUsage(const std::string& name)
	{
		if (name == "POSITION")
			return D3DDECLUSAGE_POSITION;
		if (name == "NORMAL")
			return D3DDECLUSAGE_NORMAL;
		if (name == "TEXCOORD")
			return D3DDECLUSAGE_TEXCOORD;
		if (name == "COLOR")
			return D3DDECLUSAGE_COLOR;
		if (name == "TANGENT")
			return D3DDECLUSAGE_TANGENT;
		if (name == "BINORMAL")
			return D3DDECLUSAGE_BINORMAL;
		IZ_ASSERT(false, "Unknown vertex declaration usage.");
		return D3DDECLUSAGE_POSITION;
	}

	DX9VertexArray::DX9VertexArray()
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9VertexArray requires an initialized device.");
	}

	DX9VertexArray::~DX9VertexArray()
	{
		if (VertexDeclaration)
		{
			VertexDeclaration->Release();
			VertexDeclaration = nullptr;
		}
	}

	void DX9VertexArray::Bind() const
	{
		IZ_ASSERT(VertexDeclaration, "VertexDeclaration is null.");

		DX9GraphicsContext::Device->SetVertexDeclaration(VertexDeclaration);

		for (const auto& vertex : Vertices)
			vertex->Bind();

		if (Indices)
			Indices->Bind();
	}

	void DX9VertexArray::Unbind() const
	{
		DX9GraphicsContext::Device->SetVertexDeclaration(nullptr);

		for (const auto& vertex : Vertices)
			vertex->Unbind();

		if (Indices)
			Indices->Unbind();
	}

	void DX9VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		IZ_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout.");

		Vertices.push_back(vertexBuffer);
		std::vector<D3DVERTEXELEMENT9> elements;
		uint8_t usageIndex = 0;

		std::map<D3DDECLUSAGE, uint8_t> usageIndices;

		for (const auto& buffer : Vertices)
		{
			for (const auto& element : buffer->GetLayout())
			{
				D3DVERTEXELEMENT9 ve;
				ve.Stream = 0;
				ve.Offset = static_cast<WORD>(element.Offset);
				ve.Type = static_cast<BYTE>(GetType(element.Type));
				ve.Method = D3DDECLMETHOD_DEFAULT;
				ve.Usage = static_cast<BYTE>(GetUsage(element.Name));
				ve.UsageIndex = usageIndices[static_cast<D3DDECLUSAGE>(ve.Usage)]++;
				elements.push_back(ve);
			}
		}
		elements.push_back(D3DDECL_END());

		if (VertexDeclaration)
		{
			VertexDeclaration->Release();
			VertexDeclaration = nullptr;
		}
		DX9GraphicsContext::Device->CreateVertexDeclaration(elements.data(), &VertexDeclaration);
	}

	void DX9VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		Indices = indexBuffer;
	}

	const std::vector<Ref<VertexBuffer>>& DX9VertexArray::GetVertexBuffers() const
	{
		return Vertices;
	}

	const Ref<IndexBuffer>& DX9VertexArray::GetIndexBuffer() const
	{
		return Indices;
	}
}

#include "Buffer.hpp"
#include "RendererAPI.hpp"

#include "Engine/Backend/DX9/Buffer.hpp"

namespace IzEngine
{
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 4 * 2;
		case ShaderDataType::Float3:
			return 4 * 3;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 3;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		default:
			return 0;
		}
	}

	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
	{
	}

	uint32_t BufferElement::GetComponentCount() const
	{
		switch (Type)
		{
		case ShaderDataType::Float:
			return 1;
		case ShaderDataType::Float2:
			return 2;
		case ShaderDataType::Float3:
			return 3;
		case ShaderDataType::Float4:
			return 4;
		case ShaderDataType::Mat3:
			return 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4;
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Int4:
			return 4;
		case ShaderDataType::Bool:
			return 1;
		default:
			return 0;
		}
	}

	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) : Elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	uint32_t BufferLayout::GetStride() const
	{
		return Stride;
	}

	const std::vector<BufferElement>& BufferLayout::GetElements() const
	{
		return Elements;
	}

	std::vector<BufferElement>::iterator BufferLayout::begin()
	{
		return Elements.begin();
	}

	std::vector<BufferElement>::iterator BufferLayout::end()
	{
		return Elements.end();
	}

	std::vector<BufferElement>::const_iterator BufferLayout::begin() const
	{
		return Elements.begin();
	}

	std::vector<BufferElement>::const_iterator BufferLayout::end() const
	{
		return Elements.end();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		Stride = 0;
		for (auto& element : Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			Stride += element.Size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateRef<DX9VertexBuffer>(size);
		}
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateRef<DX9VertexBuffer>(vertices, size);
		}
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return CreateRef<DX9IndexBuffer>(indices, count);
		}
		return nullptr;
	}
}

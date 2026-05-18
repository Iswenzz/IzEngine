#include "Buffer.hpp"
#include "GraphicsContext.hpp"

namespace IzEngine
{
	DX9VertexBuffer::DX9VertexBuffer(uint32_t size) : Size(size)
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9VertexBuffer requires an initialized device.");

		DX9GraphicsContext::Device->CreateVertexBuffer(size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT,
			&VertexBuffer, nullptr);
	}

	DX9VertexBuffer::DX9VertexBuffer(float* vertices, uint32_t size) : Size(size)
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9VertexBuffer requires an initialized device.");

		DX9GraphicsContext::Device->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &VertexBuffer,
			nullptr);

		void* data = nullptr;
		VertexBuffer->Lock(0, size, &data, 0);
		memcpy(data, vertices, size);
		VertexBuffer->Unlock();
	}

	DX9VertexBuffer::~DX9VertexBuffer()
	{
		if (VertexBuffer)
		{
			VertexBuffer->Release();
			VertexBuffer = nullptr;
		}
	}

	void DX9VertexBuffer::Bind() const
	{
		IZ_ASSERT(VertexBuffer, "VertexBuffer is null.");
		DX9GraphicsContext::Device->SetStreamSource(0, VertexBuffer, 0, Layout.GetStride());
	}

	void DX9VertexBuffer::Unbind() const
	{
		DX9GraphicsContext::Device->SetStreamSource(0, nullptr, 0, 0);
	}

	void DX9VertexBuffer::SetData(const void* data, uint32_t size)
	{
		IZ_ASSERT(VertexBuffer, "VertexBuffer is null.");
		IZ_ASSERT(size <= Size, "Data exceeds buffer size.");

		void* buffer = nullptr;
		VertexBuffer->Lock(0, size, &buffer, D3DLOCK_DISCARD);
		memcpy(buffer, data, size);
		VertexBuffer->Unlock();
	}

	const BufferLayout& DX9VertexBuffer::GetLayout() const
	{
		return Layout;
	}

	void DX9VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		Layout = layout;
	}

	DX9IndexBuffer::DX9IndexBuffer(uint32_t* indices, uint32_t count) : Count(count)
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9IndexBuffer requires an initialized device.");

		DX9GraphicsContext::Device->CreateIndexBuffer(count * sizeof(uint32_t), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,
			D3DPOOL_DEFAULT, &IndexBuffer, nullptr);

		void* data = nullptr;
		IndexBuffer->Lock(0, count * sizeof(uint32_t), &data, 0);
		memcpy(data, indices, count * sizeof(uint32_t));
		IndexBuffer->Unlock();
	}

	DX9IndexBuffer::~DX9IndexBuffer()
	{
		if (IndexBuffer)
		{
			IndexBuffer->Release();
			IndexBuffer = nullptr;
		}
	}

	void DX9IndexBuffer::Bind() const
	{
		IZ_ASSERT(IndexBuffer, "IndexBuffer is null.");
		DX9GraphicsContext::Device->SetIndices(IndexBuffer);
	}

	void DX9IndexBuffer::Unbind() const
	{
		DX9GraphicsContext::Device->SetIndices(nullptr);
	}

	uint32_t DX9IndexBuffer::GetCount() const
	{
		return Count;
	}
}

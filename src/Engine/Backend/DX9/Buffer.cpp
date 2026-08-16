#include "Buffer.hpp"
#include "GraphicsContext.hpp"

namespace IzEngine
{
	DX9VertexBuffer::DX9VertexBuffer(uint32_t size) : Size(size), Dynamic(true)
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9VertexBuffer requires an initialized device.");

		Create();
		GPUResource::RegisterResource(this);
	}

	DX9VertexBuffer::DX9VertexBuffer(float* vertices, uint32_t size) : Size(size), Dynamic(false)
	{
		IZ_ASSERT(DX9GraphicsContext::Device, "DX9VertexBuffer requires an initialized device.");

		if (Create())
		{
			void* data = nullptr;
			if (SUCCEEDED(VertexBuffer->Lock(0, size, &data, 0)) && data)
			{
				memcpy(data, vertices, size);
				VertexBuffer->Unlock();
			}
		}
		GPUResource::RegisterResource(this);
	}

	bool DX9VertexBuffer::Create()
	{
		const DWORD usage = Dynamic ? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY : D3DUSAGE_WRITEONLY;
		const D3DPOOL pool = Dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

		if (FAILED(DX9GraphicsContext::Device->CreateVertexBuffer(Size, usage, 0, pool, &VertexBuffer, nullptr)))
		{
			Log::WriteLine(Channel::Error, "Failed to create a {} byte vertex buffer.", Size);
			VertexBuffer = nullptr;
			return false;
		}
		return true;
	}

	DX9VertexBuffer::~DX9VertexBuffer()
	{
		GPUResource::UnregisterResource(this);
		Release();
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

	void DX9VertexBuffer::Release()
	{
		if (VertexBuffer)
		{
			VertexBuffer->Release();
			VertexBuffer = nullptr;
		}
	}

	void DX9VertexBuffer::OnBeforeReset()
	{
		if (!Dynamic)
			return;

		Release();
	}

	void DX9VertexBuffer::OnAfterReset()
	{
		if (!Dynamic)
			return;

		Create();
	}

	void DX9VertexBuffer::SetData(const void* data, uint32_t size)
	{
		IZ_ASSERT(size <= Size, "Data exceeds buffer size.");

		if (!VertexBuffer || size > Size)
			return;

		void* buffer = nullptr;
		if (FAILED(VertexBuffer->Lock(0, size, &buffer, D3DLOCK_DISCARD)) || !buffer)
			return;

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

		Indices.assign(indices, indices + count);

		if (Create())
			Upload();

		GPUResource::RegisterResource(this);
	}

	bool DX9IndexBuffer::Create()
	{
		if (FAILED(DX9GraphicsContext::Device->CreateIndexBuffer(Count * sizeof(uint32_t), D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX32, D3DPOOL_DEFAULT, &IndexBuffer, nullptr)))
		{
			Log::WriteLine(Channel::Error, "Failed to create a {} index buffer.", Count);
			IndexBuffer = nullptr;
			return false;
		}
		return true;
	}

	bool DX9IndexBuffer::Upload()
	{
		if (!IndexBuffer)
			return false;

		void* data = nullptr;
		if (FAILED(IndexBuffer->Lock(0, Count * sizeof(uint32_t), &data, 0)) || !data)
			return false;

		memcpy(data, Indices.data(), Count * sizeof(uint32_t));
		IndexBuffer->Unlock();
		return true;
	}

	DX9IndexBuffer::~DX9IndexBuffer()
	{
		GPUResource::UnregisterResource(this);
		Release();
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

	void DX9IndexBuffer::Release()
	{
		if (IndexBuffer)
		{
			IndexBuffer->Release();
			IndexBuffer = nullptr;
		}
	}

	void DX9IndexBuffer::OnBeforeReset()
	{
		Release();
	}

	void DX9IndexBuffer::OnAfterReset()
	{
		if (Create())
			Upload();
	}

	uint32_t DX9IndexBuffer::GetCount() const
	{
		return Count;
	}
}

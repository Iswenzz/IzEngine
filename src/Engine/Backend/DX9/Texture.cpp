#include "Texture.hpp"
#include "GraphicsContext.hpp"

#include "Engine/Core/System/AssetManager.hpp"

namespace IzEngine
{
	static D3DPOOL GetPool(TexturePool pool)
	{
		switch (pool)
		{
		case TexturePool::Default:
			return D3DPOOL_DEFAULT;
		case TexturePool::Managed:
			return D3DPOOL_MANAGED;
		case TexturePool::SystemMem:
			return D3DPOOL_SYSTEMMEM;
		}
		IZ_ASSERT(false, "Unknown TexturePool.");
		return D3DPOOL_DEFAULT;
	}

	static DWORD GetUsage(TextureUsage usage)
	{
		switch (usage)
		{
		case TextureUsage::None:
			return 0;
		case TextureUsage::RenderTarget:
			return D3DUSAGE_RENDERTARGET;
		case TextureUsage::Dynamic:
			return D3DUSAGE_DYNAMIC;
		}
		IZ_ASSERT(false, "Unknown TextureUsage.");
		return 0;
	}

	Ref<Texture> DX9Texture::Create(const TextureSpecification& spec)
	{
		std::string id = !spec.ID.empty() ? spec.ID : spec.Source.Path.string();

		IZ_ASSERT(!id.empty(), "TextureSpecification requires an ID or a Source.");

		if (AssetManager::Exists(id))
			return AssetManager::Get<Texture>(id);

		Ref<DX9Texture> texture = CreateRef<DX9Texture>();
		IDirect3DTexture9* dTexture = nullptr;
		IDirect3DSurface9* dSurface = nullptr;

		if (spec.Source.IsValid())
		{
			if (FAILED(D3DXCreateTextureFromFileInMemory(DX9GraphicsContext::Device, spec.Source.Data.data(),
					spec.Source.Data.size(), &dTexture)))
				return Texture::Default();
		}
		else
		{
			IZ_ASSERT(spec.Size.x > 0 && spec.Size.y > 0, "TextureSpecification requires a valid Size.");

			if (FAILED(DX9GraphicsContext::Device->CreateTexture(static_cast<UINT>(spec.Size.x),
					static_cast<UINT>(spec.Size.y), spec.Level, GetUsage(spec.Usage), D3DFMT_A8R8G8B8,
					GetPool(spec.Pool), &dTexture, nullptr)))
				return Texture::Default();
		}
		if (FAILED(dTexture->GetSurfaceLevel(0, &dSurface)))
		{
			dTexture->Release();
			return Texture::Default();
		}
		texture->Data = dTexture;
		texture->Surface = dSurface;
		texture->Spec = spec;

		GPUResource::RegisterResource(texture.get());
		return AssetManager::Add(id, texture);
	}

	DX9Texture::~DX9Texture()
	{
		GPUResource::UnregisterResource(this);
		Release();
	}

	void DX9Texture::Bind(uint32_t slot) const
	{
		DX9GraphicsContext::Device->SetTexture(slot, Data);
	}

	void DX9Texture::Unbind(uint32_t slot) const
	{
		DX9GraphicsContext::Device->SetTexture(slot, nullptr);
	}

	void DX9Texture::Release()
	{
		if (Surface)
		{
			Surface->Release();
			Surface = nullptr;
		}
		if (Data)
		{
			Data->Release();
			Data = nullptr;
		}
	}

	void DX9Texture::OnBeforeReset()
	{
		if (Spec.Pool != TexturePool::Default)
			return;

		Release();
	}

	void DX9Texture::OnAfterReset()
	{
		if (Spec.Pool != TexturePool::Default)
			return;

		IDirect3DTexture9* dTexture = nullptr;
		IDirect3DSurface9* dSurface = nullptr;

		if (FAILED(DX9GraphicsContext::Device->CreateTexture(static_cast<UINT>(Spec.Size.x),
				static_cast<UINT>(Spec.Size.y), Spec.Level, GetUsage(Spec.Usage), D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
				&dTexture, nullptr)))
		{
			Log::WriteLine(Channel::Error, "Failed to recreate texture after device reset: {}", Spec.ID);
			return;
		}
		if (FAILED(dTexture->GetSurfaceLevel(0, &dSurface)))
		{
			dTexture->Release();
			return;
		}
		Data = dTexture;
		Surface = dSurface;
	}

	vec2 DX9Texture::GetSize() const
	{
		IZ_ASSERT(Data, "Texture data is null.");

		D3DSURFACE_DESC desc;
		Data->GetLevelDesc(0, &desc);
		return { (float)desc.Width, (float)desc.Height };
	}

	void* DX9Texture::GetHandle() const
	{
		return Data;
	}
}

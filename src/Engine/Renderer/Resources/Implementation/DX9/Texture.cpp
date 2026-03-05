#include "DX9/Common.hpp"

#include "Renderer/Resources/Texture.hpp"

namespace IzEngine
{
	Texture::~Texture()
	{
		Release();
	}

	void Texture::Release()
	{
		if (Data)
		{
			reinterpret_cast<IDirect3DTexture9*>(Data)->Release();
			Data = nullptr;
		}
		if (Surface)
		{
			reinterpret_cast<IDirect3DSurface9*>(Surface)->Release();
			Surface = nullptr;
		}
	}

	vec2 Texture::GetSize()
	{
		IDirect3DTexture9* texture = reinterpret_cast<IDirect3DTexture9*>(Data);
		D3DSURFACE_DESC desc;
		texture->GetLevelDesc(0, &desc);
		return { desc.Width, desc.Height };
	}

	Ref<Texture>& Texture::Default()
	{
		IZ_ASSERT(Textures::List[TEXTURE_BLACK], "Default texture not found");

		if (!Textures::List[TEXTURE_BLACK])
		{
			Log::WriteLine(Channel::Error, "Default texture not found");
			exit(-1);
		}
		return Textures::List[TEXTURE_BLACK];
	}

	Ref<Texture>& Texture::Create(const File& file)
	{
		std::string id = file.Path.string();

		if (auto cache = Textures::List.find(id); cache != Textures::List.end())
			return cache->second;

		if (!file.IsValid())
		{
			Log::WriteLine(Channel::Error, "Texture not found: {}", file.Path.string());
			return Default();
		}
		Ref<Texture> texture = CreateRef<Texture>();
		IDirect3DTexture9* dTexture = nullptr;
		IDirect3DSurface9* dSurface = nullptr;

		if (FAILED(D3DXCreateTextureFromFileInMemory(Device::D3Device, file.Data.data(), file.Data.size(), &dTexture)))
			return Default();

		if (FAILED(dTexture->GetSurfaceLevel(0, &dSurface)))
			return Default();

		texture->Data = dTexture;
		texture->Surface = dSurface;
		return Textures::List[id] = texture;
	}

	Ref<Texture>& Texture::Create(const std::string& id, const vec2& size)
	{
		if (auto cache = Textures::List.find(id); cache != Textures::List.end())
			return cache->second;

		Ref<Texture> texture = CreateRef<Texture>();
		IDirect3DTexture9* dTexture = nullptr;
		IDirect3DSurface9* dSurface = nullptr;

		if (FAILED(Device::D3Device->CreateTexture(size.x, size.y, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &dTexture,
				nullptr)))
			return Default();

		if (FAILED(dTexture->GetSurfaceLevel(0, &dSurface)))
			return Default();

		texture->Data = dTexture;
		texture->Surface = dSurface;
		return Textures::List[id] = texture;
	}

	Ref<Texture>& Texture::Create(const std::string& id, const vec2& size, int level, int usage, int pool)
	{
		if (auto cache = Textures::List.find(id); cache != Textures::List.end())
			return cache->second;

		Ref<Texture> texture = CreateRef<Texture>();
		IDirect3DTexture9* dTexture = nullptr;
		IDirect3DSurface9* dSurface = nullptr;

		if (FAILED(Device::D3Device->CreateTexture(size.x, size.y, level, usage, D3DFMT_A8R8G8B8,
				static_cast<D3DPOOL>(pool), &dTexture, nullptr)))
			return Default();

		if (FAILED(dTexture->GetSurfaceLevel(0, &dSurface)))
			return Default();

		texture->Data = dTexture;
		texture->Surface = dSurface;
		return Textures::List[id] = texture;
	}

	void Textures::Initialize() { }

	void Textures::Shutdown()
	{
		for (auto& [id, texture] : List)
			texture->Release();

		List.clear();
	}
}

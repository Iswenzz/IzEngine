#include "DX9/Device.hpp"

#include "Renderer/Resources/Texture.hpp"

namespace IzEngine
{
	Texture::~Texture()
	{
		if (Data)
			reinterpret_cast<IDirect3DTexture9*>(Data)->Release();
		if (Surface)
			reinterpret_cast<IDirect3DSurface9*>(Surface)->Release();
	}

	Ref<Texture>& Texture::Default()
	{
		return Textures::List[TEXTURE_NULL];
	}

	Ref<Texture>& Texture::Create(const std::filesystem::path& path)
	{
		std::string id = path.stem().filename().string();

		if (auto cache = Textures::List.find(id); cache != Textures::List.end())
			return cache->second;

		if (!std::filesystem::exists(path))
		{
			Log::WriteLine(Channel::Error, "Texture not found: {}", path.string());
			return Default();
		}
		Ref<Texture> texture = CreateRef<Texture>();
		IDirect3DTexture9* dTexture = nullptr;
		IDirect3DSurface9* dSurface = nullptr;

		if (FAILED(D3DXCreateTextureFromFile(Device::D3Device, path.string().c_str(), &dTexture)))
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

		if (FAILED(Device::D3Device->CreateTexture(size.x, size.y, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &dTexture,
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

		if (FAILED(Device::D3Device->CreateTexture(size.x, size.y, level, usage, D3DFMT_X8R8G8B8,
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
		List.clear();
	}
}

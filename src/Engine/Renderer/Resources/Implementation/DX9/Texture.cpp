#include "DX9/Device.hpp"

#include "Renderer/Resources/Texture.hpp"

namespace IzEngine
{
	Ref<Texture>& Texture::Create(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
			throw std::runtime_error("File not found.");

		std::string id = path.stem().filename().string();
		if (auto cache = Textures::List.find(id); cache != Textures::List.end())
			return cache->second;

		Ref<Texture> texture = CreateRef<Texture>();
		HRESULT hr = D3DXCreateTextureFromFile(Device::D3Device, path.string().c_str(),
			reinterpret_cast<LPDIRECT3DTEXTURE9*>(&texture->Data));

		if (hr != S_OK)
			return Default();
		return Textures::List[id] = texture;
	}

	Ref<Texture>& Texture::Default()
	{
		return Textures::List[TEXTURE_NULL];
	}

	Texture::~Texture()
	{
		if (Data)
			reinterpret_cast<ID3DXFont*>(Data)->Release();
		if (Surface)
			reinterpret_cast<ID3DXFont*>(Surface)->Release();
	}

	void Textures::Initialize() { }

	void Textures::Release()
	{
		List.clear();
	}
}

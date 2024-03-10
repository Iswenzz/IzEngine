#include "Assets.hpp"
#include "Engine/Backends/DX9/Device.hpp"

namespace IW3SR::Engine
{
	void Assets::Initialize()
	{
		LoadFonts();
	}

	void Assets::Release()
	{
		Textures.clear();
		Fonts.clear();
		FontNames.clear();
	}

	void Assets::LoadFonts()
	{
		LoadFont(Environment::FontsDirectory / "OpenSans-Regular.ttf", 22);
		LoadFont(Environment::FontsDirectory / "SpaceRanger.ttf", 22);

		HDC hdc = GetDC(NULL);
		auto callback = [](const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam)
		{
			Get().FontNames.push_back(lpelf->lfFaceName);
			return TRUE;
		};
		EnumFontFamilies(hdc, NULL, static_cast<FONTENUMPROC>(callback), NULL);
		ReleaseDC(NULL, hdc);
	}

	Ref<Font> Assets::LoadFont(const std::string& name, int height)
	{
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		Ref<Font> font = CreateRef<Font>();
		HRESULT hr = D3DXCreateFont(Device::Get().D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font->Base);

		if (hr != S_OK)
			return Fonts[FONT_OPENSANS];
		return Fonts[id] = font;
	}

	Ref<Font> Assets::LoadFont(const std::filesystem::path& path, int height)
	{
		if (!std::filesystem::exists(path))
			throw std::runtime_error("File not found.");

		AddFontResource(path.string().c_str());

		std::string name = path.stem().filename().string();
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		Ref<Font> font = CreateRef<Font>();
		HRESULT hr = D3DXCreateFont(Device::Get().D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font->Base);

		if (hr != S_OK)
			return Fonts[FONT_OPENSANS];

		if (std::ranges::find(FontNames, name) == FontNames.end())
			FontNames.push_back(name);

		return Fonts[id] = font;
	}

	Ref<Texture> Assets::LoadTexture(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
			throw std::runtime_error("File not found.");

		std::string id = path.stem().filename().string();
		if (auto cache = Textures.find(id); cache != Textures.end())
			return cache->second;

		Ref<Texture> texture = CreateRef<Texture>();
		HRESULT hr = D3DXCreateTextureFromFile(Device::Get().D3Device, path.string().c_str(), &texture->Base);

		if (hr != S_OK)
			return Textures[TEXTURE_NULL];
		return Textures[id] = texture;
	}
}
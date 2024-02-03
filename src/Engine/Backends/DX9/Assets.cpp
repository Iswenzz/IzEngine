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

		HDC hdc = GetDC(NULL);
		auto callback = [](const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam)
		{
			Get().FontNames.push_back(lpelf->lfFaceName);
			return TRUE;
		};
		EnumFontFamilies(hdc, NULL, static_cast<FONTENUMPROC>(callback), NULL);
		ReleaseDC(NULL, hdc);
	}

	std::shared_ptr<Font> Assets::LoadFont(const std::string& name, int height)
	{
		if (std::ranges::find(FontNames, name) == FontNames.end())
			throw std::runtime_error("Font not found.");

		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		std::shared_ptr<Font> font = std::make_shared<Font>(id);
		D3DXCreateFont(Device::Get().D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font->Base);

		return Fonts[id] = font;
	}

	std::shared_ptr<Font> Assets::LoadFont(const std::filesystem::path& path, int height)
	{
		if (!std::filesystem::exists(path))
			throw std::runtime_error("Path not found.");

		AddFontResource(path.string().c_str());

		std::string name = path.stem().filename().string();
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		std::shared_ptr<Font> font = std::make_shared<Font>(id);
		D3DXCreateFont(Device::Get().D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font->Base);

		if (std::ranges::find(FontNames, name) == FontNames.end())
			FontNames.push_back(name.c_str());
		return Fonts[id] = font;
	}

	std::shared_ptr<Texture> Assets::LoadTexture(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
			throw std::runtime_error("Path not found.");

		std::string id = path.stem().filename().string();
		if (auto cache = Textures.find(id); cache != Textures.end())
			return cache->second;

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(id);
		D3DXCreateTextureFromFile(Device::Get().D3Device, path.string().c_str(), &texture->Base);
		return Textures[id] = texture;
	}
}
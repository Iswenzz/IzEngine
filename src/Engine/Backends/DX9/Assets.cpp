#include "Assets.hpp"

namespace IW3SR
{
	void Assets::Initialize()
	{
		LoadFonts();
	}

	void Assets::Shutdown()
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
			FontNames.push_back(lpelf->lfFaceName);
			return TRUE;
		};
		EnumFontFamilies(hdc, NULL, static_cast<FONTENUMPROC>(callback), NULL);
		ReleaseDC(NULL, hdc);
	}

	std::shared_ptr<Font> Assets::LoadFont(const std::string& name, int height)
	{
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		std::shared_ptr<Font> font = std::make_shared<Font>();
		D3DXCreateFont(dx->device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font->Base);

		if (std::ranges::find(FontNames, name) == FontNames.end())
			FontNames.push_back(name.c_str());
		return Fonts[id] = font;
	}

	std::shared_ptr<Font> Assets::LoadFont(const std::filesystem::path& path, int height)
	{
		AddFontResource(path.string().c_str());

		std::string name = path.stem().filename().string();
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		std::shared_ptr<Font> font = std::make_shared<Font>();
		D3DXCreateFont(dx->device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font->Base);

		if (std::ranges::find(FontNames, name) == FontNames.end())
			FontNames.push_back(name.c_str());
		return Fonts[id] = font;
	}

	std::shared_ptr<Texture> Assets::LoadTexture(const std::string& filePath)
	{
		if (!std::filesystem::exists(filePath))
			throw std::runtime_error("Couldn't find texture path.");

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		D3DXCreateTextureFromFile(dx->device, filePath.c_str(), &texture->Base);
		return Textures[filePath] = texture;
	}
}
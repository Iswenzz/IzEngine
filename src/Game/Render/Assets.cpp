#include "Assets.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	void Assets::Initialize()
	{
		LoadFonts();
	}

	void Assets::Shutdown()
	{
		for (auto& [name, texture] : Textures)
			texture.Reset();
		for (auto& [name, font] : Fonts)
			font.Reset();

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

	ComPtr<ID3DXFont> Assets::LoadFont(const std::string& name, int height)
	{
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		ComPtr<ID3DXFont> font;
		D3DXCreateFont(dx->device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font);

		if (std::ranges::find(FontNames, name) == FontNames.end())
			FontNames.push_back(name.c_str());
		return Fonts[id] = font.Get();
	}

	ComPtr<ID3DXFont> Assets::LoadFont(const std::filesystem::path& path, int height)
	{
		AddFontResource(path.string().c_str());

		std::string name = path.stem().filename().string();
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts.find(id); cache != Fonts.end())
			return cache->second;

		ComPtr<ID3DXFont> font;
		D3DXCreateFont(dx->device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font);

		if (std::ranges::find(FontNames, name) == FontNames.end())
			FontNames.push_back(name.c_str());
		return Fonts[id] = font;
	}

	ComPtr<IDirect3DTexture9> Assets::LoadTexture(const std::string& filePath)
	{
		if (!std::filesystem::exists(filePath))
			throw std::runtime_error("Couldn't find texture path.");

		ComPtr<IDirect3DTexture9> texture;
		D3DXCreateTextureFromFile(dx->device, filePath.c_str(), &texture);
		return Textures[filePath] = texture;
	}
}
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
		for (const auto& [name, texture] : Textures)
			texture->Release();
		for (const auto& font : Fonts)
			font->Release();
	}

	void Assets::LoadFonts()
	{
		HDC hdc = GetDC(NULL);
		LoadFont("Arial", 22);
		auto callback = [](const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam)
		{
			FontNames.push_back(lpelf->lfFaceName);
			return 1;
		};
		EnumFontFamilies(hdc, NULL, callback, NULL);
		ReleaseDC(NULL, hdc);
	}

	ID3DXFont* Assets::LoadFont(const std::string& name, int height)
	{
		ID3DXFont* font;
		D3DXCreateFont(dx->device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &font);

		if (std::find(FontNames.begin(), FontNames.end(), name) == FontNames.end())
			FontNames.push_back(name.c_str());
		Fonts.push_back(font);
		return font;
	}

	void Assets::UnloadFont(ID3DXFont* font)
	{
		auto it = std::find(Fonts.begin(), Fonts.end(), font);
		font->Release();
		Fonts.erase(it);
	}

	IDirect3DTexture9* Assets::LoadTexture(const std::string& filePath)
	{
		if (!std::filesystem::exists(filePath))
			throw std::runtime_error("Couldn't find texture path.");

		IDirect3DTexture9* texture;
		D3DXCreateTextureFromFile(dx->device, filePath.c_str(), &texture);
		return Textures[filePath] = texture;
	}
}
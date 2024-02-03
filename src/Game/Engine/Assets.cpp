#include "Assets.hpp"
#include "Engine/Backends/DX9/Assets.hpp"

namespace IW3SR::Game
{
	void Assets::Initialize()
	{
		Engine::Assets::Get().Initialize();
		LoadFonts();
	}

	void Assets::Release()
	{
		Engine::Assets::Get().Release();

		Fonts.clear();
		FontNames.clear();
	}

	void Assets::LoadFonts()
	{
		FontNames = { 
			FONT_OBJECTIVE, 
			FONT_NORMAL, 
			FONT_CONSOLE,
			FONT_SMALL, 
			FONT_SMALL_DEV, 
			FONT_BIG, 
			FONT_BIG_DEV, 
			FONT_BOLD 
		};
		for (const std::string& name : FontNames)
			LoadFont(name);
	}

	Font_s* Assets::LoadFont(const std::string& name)
	{
		auto font = R_RegisterFont(name.c_str(), name.size());
		if (!font) throw std::runtime_error("Path not found.");
		return Fonts[name] = font;
	}
}

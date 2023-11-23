#include "Assets.hpp"

namespace IW3SR::Engine
{
	void Assets::Initialize()
	{
		LoadFonts();
	}

	void Assets::Shutdown() 
	{
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
		return Fonts[name] = R_RegisterFont(name.c_str(), name.size());
	}
}

#include "Assets.hpp"

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
		if (auto cache = Fonts.find(name); cache != Fonts.end())
			return cache->second;

		const auto font = R_RegisterFont(name.c_str(), name.size());
		if (!font)
			return Fonts[FONT_OBJECTIVE];
		return Fonts[name] = font;
	}
}

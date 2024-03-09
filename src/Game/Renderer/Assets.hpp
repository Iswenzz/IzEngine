#pragma once
#include "Game/Definitions.hpp"

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

namespace IW3SR::Game
{
	/// <summary>
	/// Assets class.
	/// </summary>
	class API Assets
	{
		CLASS_SINGLETON(Assets)
	public:
		std::unordered_map<std::string, Font_s*> Fonts;
		std::vector<std::string> FontNames;

		/// <summary>
		/// Initialize the assets.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Release the assets.
		/// </summary>
		void Release();

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <returns></returns>
		Font_s* LoadFont(const std::string& name);

	private:
		/// <summary>
		/// Initialize assets.
		/// </summary>
		Assets() = default;
		virtual ~Assets() = default;

		/// <summary>
		/// Load fonts.
		/// </summary>
		void LoadFonts();
	};
}

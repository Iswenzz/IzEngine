#pragma once
#include "Game/Definitions.hpp"

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

namespace IW3SR::Engine
{
	/// <summary>
	/// Assets class.
	/// </summary>
	class API Assets
	{
	public:
		static inline std::unordered_map<std::string, Font_s*> Fonts;
		static inline std::vector<const char*> FontNames;

		/// <summary>
		/// Initialize the assets.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the assets.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Load fonts.
		/// </summary>
		static void LoadFonts();

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <returns></returns>
		static Font_s* LoadFont(const std::string& name);
	};
}

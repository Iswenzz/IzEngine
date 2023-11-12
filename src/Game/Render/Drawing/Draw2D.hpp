#pragma once
#include "Game/Definitions.hpp"
#include "Math/Math.hpp"

#include <d3dx9.h>

namespace IW3SR
{
	/// <summary>
	/// Draw 2D class.
	/// </summary>
	class API Draw2D
	{
	public:
		static inline std::unordered_map<std::string, ID3DXFont*> Fonts;
		static inline std::vector<const char*> FontNames;

		/// <summary>
		/// Initialize the fonts.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Draw text.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <param name="x">X-coordinate.</param>
		/// <param name="y">Y-coordinate.</param>
		/// <param name="size">Font size.</param>
		/// <param name="color">The color.</param>
		static void Text(const std::string& text, const std::string& font, 
			float x, float y, float size, const vec4& color);
	};
}

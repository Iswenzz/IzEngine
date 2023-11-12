#pragma once
#include "Game/Definitions.hpp"
#include "Game/Render/Assets.hpp"
#include "Math/Math.hpp"

namespace IW3SR
{
	/// <summary>
	/// Draw 2D class.
	/// </summary>
	class API Draw2D
	{
	public:
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

#pragma once
#include "Engine/Backends/DX9.hpp"
#include "Math/Common.hpp"

namespace IW3SR
{
	/// <summary>
	/// Draw 2D class.
	/// </summary>
	class API Draw2D
	{
	public:
		static inline std::vector<std::string> HorizontalAlignment = { "Left", "Center", "Right " };
		static inline std::vector<std::string> VerticalAlignment = { "Top", "Center", "Bottom" };

		/// <summary>
		/// Draw text.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <param name="x">X-coordinate.</param>
		/// <param name="y">Y-coordinate.</param>
		/// <param name="color">The color.</param>
		static void Text(const std::string& text, const std::string& font, float x, float y, const vec4& color);
	};
}

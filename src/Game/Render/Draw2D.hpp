#pragma once
#include "Game/Definitions.hpp"
#include "Utils/Math.hpp"

namespace IW3SR
{
	/// <summary>
	/// Drawing 2D class.
	/// </summary>
	class Draw2D
	{
	public:
		/// <summary>
		/// Draw a rectangle with the specified color.
		/// </summary>
		/// <param name="material">The material.</param>
		/// <param name="x">X-coordinate.</param>
		/// <param name="y">Y-coordinate.</param>
		/// <param name="w">Width of the rectangle.</param>
		/// <param name="h">Height of the rectangle.</param>
		/// <param name="color">The rect color.</param>
		static void Rect(std::string material, float x, float y, float w, float h, const vec4& color);

		/// <summary>
		/// Draw a section of a circle with a specified yaw angle, starting and ending angles, and color.
		/// </summary>
		/// <param name="material">The material.</param>
		/// <param name="start">Starting angle in radians.</param>
		/// <param name="end">Ending angle in radians.</param>
		/// <param name="yaw">Yaw angle in radians.</param>
		/// <param name="y">Y-coordinate of the circle's center.</param>
		/// <param name="h">Radius of the circle.</param>
		/// <param name="color">The color.</param>
		static void AngleYaw(std::string material, float start, float end, float yaw, 
			float y, float h, const vec4& color);

		/// <summary>
		/// Draw a line at a specified yaw angle with a given width, height, and color.
		/// </summary>
		/// <param name="material">The material.</param>
		/// <param name="angle">Angle at which to draw the line in radians.</param>
		/// <param name="yaw">Yaw angle in radians.</param>
		/// <param name="y">Y-coordinate of the line's position.</param>
		/// <param name="w">Width of the line.</param>
		/// <param name="h">Height of the line.</param>
		/// <param name="color">The color.</param>
		static void LineYaw(std::string material, float angle, float yaw, float y, 
			float w, float h, const vec4& color);

		/// <summary>
		/// Draw a text.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <param name="pos">XY-coordinates of screen.</param>
		/// <param name="size">Text/Font size.</param>
		static void Text(std::string text, Font_s* font, const vec2& pos, const vec2& size);

		/// <summary>
		/// Draw a text.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <param name="x">X-coordinate.</param>
		/// <param name="y">Y-coordinate.</param>
		/// <param name="size">Text/Font size.</param>
		/// <param name="color">The color.</param>
		static void Text(std::string text, Font_s* font, float x, float y, float size);
	};
}

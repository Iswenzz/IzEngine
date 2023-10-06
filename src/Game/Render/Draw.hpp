#pragma once
#include "Game/Definitions.hpp"
#include "Game/Render/Draw/Lines.hpp"
#include "Utils/Math.hpp"

#include <array>

namespace IW3SR
{
	/// <summary>
	/// Drawing class.
	/// </summary>
	class Draw
	{
	public:
		static inline std::array<GfxPointVertex, 2735> LinesVerts{ };
		static inline int LinesCount = 0;
		static inline int LinesWidth = 2;
		static inline int LinesDepthTest = true;

		/// <summary>
		/// Draw a line.
		/// </summary>
		/// <param name="start">The start point.</param>
		/// <param name="end">The end point.</param>
		/// <param name="color">The color.</param>
		static void Line(const vec3& start, const vec3& end, const vec4& color);

		/// <summary>
		/// Draw a 2D Rect.
		/// </summary>
		/// <param name="x">The X coord.</param>
		/// <param name="y">The Y coord.</param>
		/// <param name="w">The width.</param>
		/// <param name="h">The height.</param>
		/// <param name="color">The color.</param>
		static void Rect2D(float x, float y, float w, float h, const vec4& color);

		/// <summary>
		/// Draw frame.
		/// </summary>
		static void Frame();
	};
}

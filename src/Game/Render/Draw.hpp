#pragma once
#include "Game/Definitions.hpp"
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
		static void Line(glm::vec3 start, glm::vec3 end, glm::vec4 color);

		/// <summary>
		/// Draw frame.
		/// </summary>
		static void Frame();
	};
}

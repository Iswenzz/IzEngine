#pragma once
#include "Game/Definitions.hpp"
#include "Game/Engine/Assets.hpp"

#include "Engine/Core/Math.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Draw 3D class.
	/// </summary>
	class API Draw3D
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
		/// Draw frame.
		/// </summary>
		static void Render();
	};
}

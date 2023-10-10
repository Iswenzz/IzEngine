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
		/// Draw a HUD.
		/// </summary>
		/// <param name="x">The X coord.</param>
		/// <param name="y">The Y coord.</param>
		/// <param name="w">The width.</param>
		/// <param name="h">The height.</param>
		/// <param name="color">The color.</param>
		static void HUD(float x, float y, float w, float h, const vec4& color);
	};
}

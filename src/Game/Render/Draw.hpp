#pragma once
#include "Game/Definitions.hpp"
#include "Draw2D.hpp"
#include "Draw3D.hpp"

namespace IW3SR
{
	/// <summary>
	/// Drawing class.
	/// </summary>
	class Draw
	{
	public:
		/// <summary>
		/// Draw a box.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="size">The box size.</param>
		/// <param name="color">The box color.</param>
		/// <param name="thickness">The box thickness.</param>
		static void Box(const vec3& position, const vec3& size, const vec4& color, int thickness);
	};
}

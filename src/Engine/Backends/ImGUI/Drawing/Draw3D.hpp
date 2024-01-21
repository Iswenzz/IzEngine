#pragma once
#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Core/Math.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Draw 3D class.
	/// </summary>
	class API Draw3D
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

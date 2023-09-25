#pragma once
#include "Game/Definitions.hpp"
#include <glm/glm.hpp>

namespace IW3SR
{
	/// <summary>
	/// Rendering class.
	/// </summary>
	class Render
	{
	public:
		/// <summary>
		/// Initialize the rendering class.
		/// </summary>
		Render() = default;
		~Render() = default;
		
		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		/// <summary>
		/// Draw a line.
		/// </summary>
		/// <param name="from">Point A.</param>
		/// <param name="to">Point B.</param>
		/// <param name="color">Line color.</param>
		void DrawLine(glm::vec3 from, glm::vec3 to, glm::vec4 color);
	};
}

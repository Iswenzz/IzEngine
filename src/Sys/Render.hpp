#pragma once
#include "Game/Definitions.hpp"

#include <glm/glm.hpp>
#include <vector>

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
		/// <param name="start">Point A.</param>
		/// <param name="end">Point B.</param>
		/// <param name="color">Line color.</param>
		/// <param name="depthTest">Depth test.</param>
		/// <param name="verts">The verts.</param>
		int DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color, bool depthTest,
			std::vector<GfxPointVertex> verts);
	};
}

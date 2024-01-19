#pragma once
#include "Game/Definitions.hpp"
#include "Engine/Core/Math.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Lines class.
	/// </summary>
	class API Lines : public IObject
	{
	public:
		std::vector<GfxPointVertex> Verts{ };
		int Count = 0;
		int Width = 2;
		int Limit = 2735;
		bool DepthTest = true;

		/// <summary>
		/// Initialize lines.
		/// </summary>
		/// <param name="width">The lines width.</param>
		/// <param name="limit">The lines size limit.</param>
		/// <param name="depthTest">The depth test.</param>
		Lines(int width, int limit, bool depthTest);
		~Lines() = default;

		/// <summary>
		/// Add a line.
		/// </summary>
		/// <param name="start">The start point.</param>
		/// <param name="end">The end point.</param>
		/// <param name="color">The color.</param>
		void Add(const vec3& start, const vec3& end, const vec4& color);

		/// <summary>
		/// Draw frame.
		/// </summary>
		void Render();
	};
}
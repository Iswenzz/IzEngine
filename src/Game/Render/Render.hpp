#pragma once
#include "Game/Definitions.hpp"
#include "Utils/Math.hpp"

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
		Render();
		~Render() = default;
		
		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		/// <summary>
		/// Draw 2D.
		/// </summary>
		/// <param name="isScoreboardVisible">Scoreboard visible.</param>
		void Draw(bool isScoreboardVisible);
	};
}

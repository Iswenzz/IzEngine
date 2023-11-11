#pragma once
#include "Game/Definitions.hpp"
#include "Math/Math.hpp"

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
		/// Draws 3D.
		/// </summary>
		/// <param name="input">Input commands for rendering.</param>
		/// <param name="viewInfo">View information for the graphics.</param>
		/// <param name="src">Source state for the graphics command buffer.</param>
		/// <param name="buf">Graphics command buffer state.</param>
		static void Draw3D(GfxCmdBufInput* input, GfxViewInfo* viewInfo,
			GfxCmdBufSourceState* src, GfxCmdBufState* buf);

		/// <summary>
		/// Draw 2D.
		/// </summary>
		/// <param name="scoreboard">Scoreboard visible.</param>
		static void Draw2D(bool scoreboard);

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();
	};
}

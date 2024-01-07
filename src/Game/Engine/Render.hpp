#pragma once
#include "Game/Definitions.hpp"
#include "Game/Engine/Backends/D3D9EX.hpp"
#include "Math/Common.hpp"

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
		/// Patch the game.
		/// </summary>
		void Patch();

		/// <summary>
		/// Initialize the renderer.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the renderer.
		/// </summary>
		/// <param name="window">The window.</param>
		static void Shutdown(int window);

		/// <summary>
		/// Draws 3D.
		/// </summary>
		/// <param name="cmd">Render command.</param>
		/// <param name="viewInfo">View information for the graphics.</param>
		/// <param name="src">Source state for the graphics command buffer.</param>
		/// <param name="buf">Graphics command buffer state.</param>
		static void Draw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo,
			GfxCmdBufSourceState* src, GfxCmdBufState* buf);

		/// <summary>
		/// Draw 2D.
		/// </summary>
		/// <param name="localClientNum">Local client index.</param>
		static void Draw2D(int localClientNum);

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();
	};
}

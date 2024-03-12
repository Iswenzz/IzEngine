#pragma once
#include "Game/Definitions.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Renderer class.
	/// </summary>
	class Renderer
	{
		CLASS_SINGLETON(Renderer)
	public:
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
		/// Draw 2D.
		/// </summary>
		/// <param name="localClientNum">Local client num.</param>
		static void Draw2D(int localClientNum);

		/// <summary>
		/// Draw 3D.
		/// </summary>
		/// <param name="cmd">Render command.</param>
		/// <param name="viewInfo">View information for the graphics.</param>
		/// <param name="src">Source state for the graphics command buffer.</param>
		/// <param name="buf">Graphics command buffer state.</param>
		static void Draw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo,
			GfxCmdBufSourceState* src, GfxCmdBufState* buf);

		/// <summary>
		/// Render commands.
		/// </summary>
		/// <param name="cmds">The commands.</param>
		static void Commands(void* cmds);

		/// <summary>
		/// Render frame.
		/// </summary>
		void Render();

	private:
		/// <summary>
		/// Initialize the renderer.
		/// </summary>
		Renderer() = default;
		~Renderer() = default;
	};
}

#pragma once
#include "Core/Math.hpp"

namespace IzEngine
{
	/// <summary>
	/// Renderer class.
	/// </summary>
	class Renderer
	{
	public:
		static inline bool Active = false;

		/// <summary>
		/// Initialize renderer.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Initialize assets.
		/// </summary>
		static void InitializeAssets();

		/// <summary>
		/// Shutdown renderer.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Shutdown assets.
		/// </summary>
		static void ShutdownAssets();

		/// <summary>
		/// Resize the renderer buffer.
		/// </summary>
		/// <param name="size">The size.</param>
		static void Resize(const vec2& size);

		/// <summary>
		/// Begin frame.
		/// </summary>
		static void Begin();

		/// <summary>
		/// End frame.
		/// </summary>
		static void End();

		/// <summary>
		/// Render frame.
		/// </summary>
		static void Frame();
	};
}

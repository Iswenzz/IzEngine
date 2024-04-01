#pragma once
#include "Game/Base.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Game patch.
	/// </summary>
	class Patch
	{
	public:
		/// <summary>
		/// Initialize patch.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown patch.
		/// </summary>
		static void Shutdown();

	private:
		/// <summary>
		/// Patch definitions.
		/// </summary>
		static void Definitions();

		/// <summary>
		/// Renderer patch.
		/// </summary>
		static void Renderer();

		/// <summary>
		/// System patch.
		/// </summary>
		static void System();

		/// <summary>
		/// CoD4X client modifications.
		/// </summary>
		static void CoD4X();

		/// <summary>
		/// Hook application.
		/// </summary>
		static void Hook();

		/// <summary>
		/// Unhook application.
		/// </summary>
		static void Unhook();
	};
}

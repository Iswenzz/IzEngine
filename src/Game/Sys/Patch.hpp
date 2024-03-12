#pragma once
#include "Game/Definitions.hpp"

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
	};
}

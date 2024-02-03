#pragma once
#include "Engine/Sys/Win32.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Game patch.
	/// </summary>
	class Patch
	{
		CLASS_SINGLETON(Patch)
	public:
		/// <summary>
		/// Initialize patch.
		/// </summary>
		void Initialize();

	private:
		/// <summary>
		/// Initialize patch.
		/// </summary>
		Patch() = default;
		~Patch() = default;

		/// <summary>
		/// Renderer patch.
		/// </summary>
		void Renderer();

		/// <summary>
		/// CoD4X patch.
		/// </summary>
		void CoD4X();

		/// <summary>
		/// System patch.
		/// </summary>
		void System();
	};
}

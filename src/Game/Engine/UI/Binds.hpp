#pragma once
#include "Game/Engine/GUI.hpp"

namespace IW3SR::Game::UI
{
	/// <summary>
	/// Binds window.
	/// </summary>
	class Binds : public Window
	{
	public:
		/// <summary>
		/// Initialize the binds window.
		/// </summary>
		Binds();
		virtual ~Binds() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void Render();
	};
}

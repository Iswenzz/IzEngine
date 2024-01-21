#pragma once
#include "Game/Engine/GUI.hpp"

namespace IW3SR::Game::UI
{
	/// <summary>
	/// Settings window.
	/// </summary>
	class Settings : public Window
	{
	public:
		/// <summary>
		/// Initialize the settings window.
		/// </summary>
		Settings();
		virtual ~Settings() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void Render();
	};
}

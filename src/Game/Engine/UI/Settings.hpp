#pragma once
#include "Game/Engine/GUI.hpp"

namespace IW3SR::UI
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
		void Frame();

		NLOHMANN_SERIALIZE_DERIVED_EMPTY(Settings, Window)
	};
}

#pragma once
#include "Game/Render/GUI.hpp"

namespace IW3SR
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

		NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE_EMPTY(Settings, Window);
	};
}

#pragma once
#include "Game/Engine/GUI.hpp"

namespace IW3SR::UI
{
	/// <summary>
	/// Main toolbar.
	/// </summary>
	class Toolbar : public Window
	{
	public:
		/// <summary>
		/// Initialize the Toolbar.
		/// </summary>
		Toolbar();
		virtual ~Toolbar() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		NLOHMANN_SERIALIZE_DERIVED_EMPTY(Toolbar, Window)
	};
}

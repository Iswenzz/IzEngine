#pragma once
#include "Game/Render/GUI.hpp"

namespace IW3SR
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

		NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE_EMPTY(Toolbar, Window)
	};
}

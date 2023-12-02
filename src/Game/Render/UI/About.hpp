#pragma once
#include "Game/Render/GUI.hpp"

namespace IW3SR
{
	/// <summary>
	/// About window.
	/// </summary>
	class About : public Window
	{
	public:
		/// <summary>
		/// Initialize the about window.
		/// </summary>
		About();
		virtual ~About() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE_EMPTY(About, Window)
	};
}

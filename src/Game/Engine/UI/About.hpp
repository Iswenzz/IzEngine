#pragma once
#include "Game/Engine/GUI.hpp"

namespace IW3SR::UI
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
		void Render();
	};
}

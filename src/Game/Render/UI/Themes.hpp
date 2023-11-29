#pragma once
#include "Game/Render/GUI.hpp"

namespace IW3SR
{
	/// <summary>
	/// Themes window.
	/// </summary>
	class Themes : public Window
	{
	public:
		/// <summary>
		/// Initialize the themes window.
		/// </summary>
		Themes();
		virtual ~Themes() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE_EMPTY(Themes, Window);
	};
}

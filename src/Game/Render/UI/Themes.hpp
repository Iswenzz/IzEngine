#pragma once
#include "Game/Render/GUI.hpp"

namespace IW3SR::UI
{
	/// <summary>
	/// Themes window.
	/// </summary>
	class Themes : public Window
	{
	public:
		ImGuiStyle Style;
		std::tuple<ImColor, ImColor> Rainbow;

		/// <summary>
		/// Initialize the themes window.
		/// </summary>
		Themes();
		virtual ~Themes() = default;

		/// <summary>
		/// Initialize themes.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Default theme.
		/// </summary>
		void Default();

		/// <summary>
		/// Compute rainbow color.
		/// </summary>
		void ComputeRainbow();

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE(Themes, Window, Style)
	};
}

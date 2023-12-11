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
		std::string Theme;
		std::unordered_map<std::string, ImGuiStyle> Styles;
		std::vector<std::string> Names;
		std::tuple<ImColor, ImColor> Rainbow;
		int Index = 0;

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
		/// Compute rainbow color.
		/// </summary>
		void ComputeRainbow();

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE(Themes, Window, Theme, Styles)
	};
}

#pragma once
#include "ImGUI/Drawing/Frame.hpp"

#include <tuple>

namespace IzEngine::UC
{
	/// <summary>
	/// Themes frame.
	/// </summary>
	class Themes : public Frame
	{
	public:
		ImGuiStyle Style;
		ImPlotStyle PlotStyle;

		/// <summary>
		/// Initialize the themes frame.
		/// </summary>
		Themes();
		virtual ~Themes() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void OnRender() override;

	private:
		/// <summary>
		/// Default theme.
		/// </summary>
		void Default();

		/// <summary>
		/// Apply theme.
		/// </summary>
		void Apply();

		SERIALIZE_POLY(Themes, Frame, Style, PlotStyle)
	};
}

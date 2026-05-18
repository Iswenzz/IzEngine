#pragma once
#include "Engine/Backend/ImGUI/Common.hpp"

namespace IzEngine::UC
{
	class Themes : public Frame
	{
	public:
		ImGuiStyle Style;
		ImPlotStyle PlotStyle;

		Themes();
		virtual ~Themes() = default;

		void OnRender() override;

	private:
		void Default();
		void Apply();

		SERIALIZE_POLY(Themes, Frame, Style, PlotStyle)
	};
}

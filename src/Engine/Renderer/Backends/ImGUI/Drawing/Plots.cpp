#include "Plots.hpp"

namespace IW3SR::Engine
{
	Plots::Plots() : Window(UUID())
	{
		SetRect(0, -100, 200, 100);
		SetRectAlignment(HORIZONTAL_LEFT, VERTICAL_BOTTOM);
		SetDesigner(true);
	}

	void Plots::Begin(ImGuiWindowFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		Window::Begin(flags | ImGuiWindowFlags_Widget);
		ImGui::PopStyleVar();
	}
}

#include "Plots.hpp"
#include "Engine/Backends/ImGUI/UI.hpp"

namespace IW3SR::Engine
{
	Plots::Plots() : Window(Utils::UUID()) 
	{
		SetRect({ 0, 380 }, { 160, 100 });
	}

	void Plots::Begin(ImGuiWindowFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		Window::Begin(ImGuiWindowFlags_Graph);
		ImGui::PopStyleVar();
	}
}

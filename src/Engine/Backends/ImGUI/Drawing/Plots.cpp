#include "Plots.hpp"
#include "Engine/Backends/ImGUI/Components.hpp"

namespace IW3SR::Engine
{
	Plots::Plots() : Window("##Plots") { }

	void Plots::Begin(ImGuiWindowFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		Window::Begin(ImGuiWindowFlags_Graph);
		ImGui::PopStyleVar();
	}
}

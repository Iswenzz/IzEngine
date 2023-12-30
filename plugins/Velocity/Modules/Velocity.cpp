#include "Velocity.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	Velocity::Velocity() : Module("sr.player.velocity", "Velocity", "Player")
	{
		VelocityText = Text("0", "Arial", 0, 2, 1.4, { 0, 1, 1, 1 });
		VelocityText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		VelocityText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);

		MaxVelocityText = Text("0", "Arial", 0, 20, 1.4, { 1, 0, 0, 1 });
		MaxVelocityText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		MaxVelocityText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);

		Plots = Window("Plots");
		ResetKey = KeyListener('R');
		ShowMaxVelocity = true;
		ShowPlot = false;
	}

	void Velocity::OnMenu()
	{
		ImGui::Checkbox("Graph", &ShowPlot);
		ImGui::Checkbox("Max Velocity", &ShowMaxVelocity);

		if (ShowMaxVelocity)
			ImGui::Keybind("Reset", &ResetKey.Key, { 150, 0 });

		VelocityText.Menu("Velocity");
		MaxVelocityText.Menu("Max Velocity");
	}

	void Velocity::OnFrame()
	{
		if (ResetKey.IsPressed())
			Max = 0;

		Values.Add(Value);
		Value = vec2(pmove->ps->velocity).Length();
		if (Value > Max) 
			Max = Value;

		VelocityText.Value = std::to_string(Value);
		MaxVelocityText.Value = std::format("({})", Max);

		VelocityText.Render();
		if (ShowMaxVelocity)
			MaxVelocityText.Render();

		Plots.Begin(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
		if (ImPlot::BeginPlot("##Velocity", Plots.RenderSize))
		{
			constexpr int axisFlags = ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks
				| ImPlotAxisFlags_NoGridLines;

			ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(VelocityText.Color));
			ImPlot::SetupAxes(nullptr, nullptr, axisFlags, axisFlags);
			ImPlot::SetupAxisLimits(ImAxis_X1, 0, Values.Size(), ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, Max * 1.5, ImGuiCond_Always);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			ImPlot::PlotShaded("Velocity", Values.Get(), Values.Size(), -INFINITY, 1, 0, 0, Values.Offset);
			ImPlot::PlotLine("Velocity", Values.Get(), Values.Size(), 1, 0, 0, Values.Offset);
			ImPlot::PopStyleColor();
			ImPlot::EndPlot();
		}
		Plots.End();
	}
}

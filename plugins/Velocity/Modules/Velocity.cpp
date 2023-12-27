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

		Value = vec2(pmove->ps->velocity).Length();
		Values.push_back(Value);
		if (Value > Max) 
			Max = Value;

		VelocityText.Value = std::to_string(Value);
		MaxVelocityText.Value = std::format("({})", Max);

		VelocityText.Render();
		if (ShowMaxVelocity)
			MaxVelocityText.Render();

		if (ShowPlot && ImPlot::BeginPlot("Velocity plot"))
		{
			ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4{ 0.96078431372, 0.15294117647, 0.15294117647, 1 });
			ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_AutoFit);
			ImPlot::SetupAxis(ImAxis_Y1, "Velocity", ImPlotAxisFlags_AutoFit);
			ImPlot::PlotLine("Player speed", Values.data(), Values.size(), 1, 0, ImPlotLineFlags_Shaded);
			ImPlot::PopStyleColor();
			ImPlot::EndPlot();
		}
	}
}

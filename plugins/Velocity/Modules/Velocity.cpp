#include "Velocity.hpp"

namespace IW3SR
{
	Velocity::Velocity() : Module("sr.player.velocity", "Velocity", "Player")
	{
		Graph = Window("Graph");
		ResetKey = KeyListener('R');
		ShowAverage = false;
		ShowMax = false;
		ShowGraph = false;
	}

	void Velocity::Initialize()
	{
		VelocityText = Text("0", "Arial", 0, 2, 1.4, { 0, 1, 1, 1 });
		VelocityText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		VelocityText.SetAlignment(ALIGN_CENTER, ALIGN_BOTTOM);

		AverageText = Text("0", "Arial", 50, 2, 1.4, { 1, 1, 0, 1 });
		AverageText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		AverageText.SetAlignment(ALIGN_CENTER, ALIGN_BOTTOM);

		MaxText = Text("0", "Arial", 100, 2, 1.4, { 1, 0, 0, 1 });
		MaxText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		MaxText.SetAlignment(ALIGN_CENTER, ALIGN_BOTTOM);
	}

	void Velocity::OnMenu()
	{
		ImGui::Checkbox("Average Velocity", &ShowAverage);
		ImGui::SameLine();
		ImGui::Checkbox("Max Velocity", &ShowMax);
		if (ShowMax)
		{
			ImGui::SameLine();
			ImGui::Keybind("Reset", &ResetKey.Key, { 150, 0 });
		}
		ImGui::Checkbox("Display Graph", &ShowGraph);

		VelocityText.Menu("Velocity");
		AverageText.Menu("Average Velocity");
		MaxText.Menu("Max Velocity");
	}

	void Velocity::OnRender()
	{
		Value = vec2(pmove->ps->velocity).Length();
		Values.Add(Value);
		Average = std::accumulate(Values.Data.begin(), Values.Data.end(), 0) / Values.Size();
		Averages.Add(Average);

		if (Value > Max)
			Max = Value;
		Maxs.Add(Max);

		VelocityText.Value = std::to_string(Value);
		AverageText.Value = std::to_string(Average);
		MaxText.Value = std::to_string(Max);

		VelocityText.Render();
		if (ShowAverage)
			AverageText.Render();
		if (ShowMax)
			MaxText.Render();

		if (ResetKey.IsPressed())
		{
			Value = 0;
			Average = 0;
			Max = 0;

			Values.Clear();
			Averages.Clear();
			Maxs.Clear();
		}
		if (ShowGraph)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			Graph.Begin(ImGuiWindowFlags_Graph);
			if (ImPlot::BeginPlot("##Velocity", Graph.RenderSize))
			{
				ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(VelocityText.Color));
				ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_Canvas, ImPlotAxisFlags_Canvas);
				ImPlot::SetupAxisLimits(ImAxis_X1, 0, Values.Size(), ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0, Max * 1.5, ImGuiCond_Always);

				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PlotShaded("Velocity", Values.Get(), Values.Size(), -INFINITY, 1, 0, 0, Values.Offset);
				ImPlot::PlotLine("Velocity", Values.Get(), Values.Size(), 1, 0, 0, Values.Offset);

				if (ShowAverage)
				{
					ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(AverageText.Color));
					ImPlot::PlotLine("Average", Averages.Get(), Averages.Size(), 1, 0, 0, Averages.Offset);
					ImPlot::PopStyleColor();
				}
				if (ShowMax)
				{
					ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(MaxText.Color));
					ImPlot::PlotLine("Max", Maxs.Get(), Maxs.Size(), 1, 0, 0, Maxs.Offset);
					ImPlot::PopStyleColor();
				}
				ImPlot::PopStyleColor();
				ImPlot::EndPlot();
			}
			Graph.End();
			ImGui::PopStyleVar();
		}
	}
}

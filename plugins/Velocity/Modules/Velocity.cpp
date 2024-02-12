#include "Velocity.hpp"

namespace IW3SR::Addons
{
	Velocity::Velocity() : Module("sr.player.velocity", "Velocity", "Player")
	{
		ResetKey = KeyListener('R');
		ShowAverage = false;
		ShowMax = false;
		ShowGround = false;
		ShowGroundTime = false;
		ShowGraph = true;
	}

	void Velocity::Initialize()
	{
		Graph = Plots();

		VelocityText = Text("0", FONT_SPACERANGER, 0, 2, 1.4, { 0, 1, 1, 1 });
		VelocityText.SetRectAlignment(HORIZONTAL_CENTER, VERTICAL_TOP);
		VelocityText.SetAlignment(ALIGN_CENTER, ALIGN_BOTTOM);

		AverageText = Text("0", FONT_SPACERANGER, 50, 2, 1.4, { 1, 1, 0, 1 });
		AverageText.SetRectAlignment(HORIZONTAL_CENTER, VERTICAL_TOP);
		AverageText.SetAlignment(ALIGN_CENTER, ALIGN_BOTTOM);

		MaxText = Text("0", FONT_SPACERANGER, 100, 2, 1.4, { 1, 0, 0, 1 });
		MaxText.SetRectAlignment(HORIZONTAL_CENTER, VERTICAL_TOP);
		MaxText.SetAlignment(ALIGN_CENTER, ALIGN_BOTTOM);

		GroundText = Text("0", FONT_SPACERANGER, -50, 2, 1.4, { 0, 1, 0, 1 });
		GroundText.SetRectAlignment(HORIZONTAL_CENTER, VERTICAL_TOP);
		GroundText.SetAlignment(ALIGN_CENTER, ALIGN_BOTTOM);
	}

	void Velocity::Compute()
	{
		bool onGround = PMove::OnGround();
		static bool prevOnGround = onGround;
		bool landed = onGround && !prevOnGround;

		Value = vec2(pmove->ps->velocity).Length();
		BufferValues.Add(Value);

		if (landed)
		{
			Averages.Add(Value);
			Average = std::accumulate(Averages.Data.begin(), Averages.Data.end(), 0) / Averages.Offset;
			GroundAverages.Add(GroundTime);
			GroundAverage = std::accumulate(GroundAverages.Data.begin(), GroundAverages.Data.end(), 0) / GroundAverages.Offset;
			GroundTime = 0;
		}
		if (onGround)
			GroundTime += UI::Get().DeltaTimeMS();

		BufferAverages.Add(Average);

		Max = Value > Max ? Value : Max;
		BufferMaxs.Add(Max);

		Ground = ShowGroundTime ? GroundTime : GroundAverage;
		Ground = Ground < 1000 ? Ground : 1000;
		BufferGrounds.Add(Ground);

		prevOnGround = onGround;
	}

	void Velocity::OnMenu()
	{
		ImGui::Checkbox("Average", &ShowAverage);
		ImGui::Checkbox("Max", &ShowMax);

		if (ShowMax)
		{
			ImGui::SameLine();
			ImGui::Keybind("Reset", &ResetKey.Key);
		}
		ImGui::Checkbox("Ground", &ShowGround);

		if (ShowGround)
		{
			ImGui::SameLine();
			ImGui::Checkbox("Time", &ShowGroundTime);
		}
		ImGui::Checkbox("Display Graph", &ShowGraph);

		VelocityText.Menu("Velocity");
		AverageText.Menu("Average");
		MaxText.Menu("Max");
		GroundText.Menu("Ground");
		Graph.Menu("Graph");
	}

	void Velocity::OnRender()
	{
		Compute();

		VelocityText.Value = std::to_string(Value);
		AverageText.Value = std::to_string(Average);
		MaxText.Value = std::to_string(Max);
		GroundText.Value = std::to_string(Ground);

		VelocityText.Render();
		if (ShowAverage)
			AverageText.Render();
		if (ShowMax)
			MaxText.Render();
		if (ShowGround)
			GroundText.Render();

		if (ResetKey.IsPressed())
		{
			Max = 0;
			BufferMaxs.Clear();

			GroundAverage = 0;
			GroundTime = 0;
			BufferGrounds.Clear();
		}
		if (ShowGraph)
		{
			Graph.Begin();
			if (ImPlot::BeginPlot("##Velocity", Graph.RenderSize))
			{
				ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(VelocityText.Color));
				ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_Canvas, ImPlotAxisFlags_Canvas);
				ImPlot::SetupAxisLimits(ImAxis_X1, 0, BufferValues.Size(), ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0, Max * 1.5, ImGuiCond_Always);

				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PlotShaded("Velocity", BufferValues.Get(), BufferValues.Size(), -INFINITY, 1, 0, 0, BufferValues.Offset);
				ImPlot::PlotLine("Velocity", BufferValues.Get(), BufferValues.Size(), 1, 0, 0, BufferValues.Offset);

				if (ShowAverage)
				{
					ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(AverageText.Color));
					ImPlot::PlotLine("Average", BufferAverages.Get(), BufferAverages.Size(), 1, 0, 0, BufferAverages.Offset);
					ImPlot::PopStyleColor();
				}
				if (ShowMax)
				{
					ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(MaxText.Color));
					ImPlot::PlotLine("Max", BufferMaxs.Get(), BufferMaxs.Size(), 1, 0, 0, BufferMaxs.Offset);
					ImPlot::PopStyleColor();
				}
				if (ShowGround)
				{
					ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(GroundText.Color));
					ImPlot::PlotLine("Ground", BufferGrounds.Get(), BufferGrounds.Size(), 1, 0, 0, BufferGrounds.Offset);
					ImPlot::PopStyleColor();
				}
				ImPlot::PopStyleColor();
				ImPlot::EndPlot();
			}
			Graph.End();
		}
	}
}

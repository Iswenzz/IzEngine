#include "FPS.hpp"

namespace IW3SR
{
	FPS::FPS() : Module("sr.player.fps", "FPS", "Player")
	{
		FPSText = Text("0", "Arial", 0, 20, 1.4, { 1, 1, 1, 1 });
		FPSText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		FPSText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);

		ShowPlot = false;
	}

	void FPS::OnMenu()
	{	
		ImGui::Checkbox("Graph", &ShowPlot);
		FPSText.Menu("Text", true);
	}

	void FPS::OnFrame()
	{
		const int fps = Dvar::Get<int>("com_maxfps");
		FPSText.Value = std::to_string(fps);
		FPSText.Render();
		Values.push_back(fps);

		if (ShowPlot && ImPlot::BeginPlot("FPS"))
		{
			ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_AutoFit);
		    ImPlot::SetupAxis(ImAxis_Y1, "FPS");
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1000);
			ImPlot::PlotLine("FPS", Values.data(), Values.size(), 1, 0, ImPlotLineFlags_Shaded);
			ImPlot::EndPlot();
		}
	}
}
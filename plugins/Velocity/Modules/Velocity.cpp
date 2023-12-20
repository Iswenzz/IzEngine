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
		DrawVelocityPlot = false;
	}

	void Velocity::OnMenu()
	{
		ImGui::Checkbox("Draw velocity plot", &DrawVelocityPlot);
		ImGui::Checkbox("Show max velocity", &ShowMaxVelocity);

		if (ShowMaxVelocity)
			ImGui::Keybind("Reset key", &ResetKey.Key, { 150, 0 });

		VelocityText.Menu("Text");
		MaxVelocityText.Menu("Text");
	}

	void Velocity::OnFrame()
	{
		const int velocity = vec2(pmove->ps->velocity).Length();
		static int maxVelocity;

		if (velocity > maxVelocity)
			maxVelocity = velocity;
		if (ResetKey.IsPressed())
			maxVelocity = 0;

		VelocityText.Value = std::to_string(velocity);
		MaxVelocityText.Value = std::to_string(maxVelocity);
		VelocityText.Render();
		if (ShowMaxVelocity)
			MaxVelocityText.Render();

		if (!DrawVelocityPlot)
			return;

		static std::vector<int> vel, samples;
		vel.push_back(velocity);
		samples.push_back(clients->serverTime);

		const ImVec2 size = ImGui::GetContentRegionAvail();
		if (ImPlot::BeginPlot("Velocity plot", nullptr, nullptr, size, ImPlotFlags_NoChild))
		{
			ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4{ 0.96078431372, 0.15294117647, 0.15294117647, 1 });
			ImPlot::SetupAxis(ImAxis_X1, "Frame time", ImPlotAxisFlags_AutoFit);
			ImPlot::SetupAxis(ImAxis_Y1, "Velocity", ImPlotAxisFlags_AutoFit);
			ImPlot::PlotLine("Player speed", samples.data(), vel.data(), vel.size(), ImPlotLineFlags_Shaded);
			ImPlot::PopStyleColor();
			ImPlot::EndPlot();
		}
	}
}

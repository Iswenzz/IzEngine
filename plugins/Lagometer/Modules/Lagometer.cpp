#include "Lagometer.hpp"

namespace IW3SR::Addons
{
	Lagometer::Lagometer() : Module("sr.player.lagometer", "Lagometer", "Player")
	{
		Graph = Plots();
		SnapColor = { 0, 0, 1, 1 };
		SnapDelayColor = { 1, 1, 0, 1 };
		SnapFlagColor = { 0, 1, 0, 1 };
		SnapFlagDropColor = { 1, 1, 0, 1 };
		PingColor = { 1, 0, 0, 1 };
		ShowSnap = true;
		ShowSnapFlag = true;
		ShowPing = true;
	}

	void Lagometer::OnMenu()
	{
		ImGui::Checkbox("Snapshot", &ShowSnap);
		ImGui::Checkbox("Snapshot Flags", &ShowSnapFlag);
		ImGui::Checkbox("Ping", &ShowPing);
		Graph.Menu("Graph");
	}

	void Lagometer::OnRender()
	{
		const int snapRange = Graph.Size.y / 3;
		const int snapScale = snapRange / 300;

		int snap = (cgs->time - cgs->latestSnapshotTime) * snapScale;
		int snapFlags = cgs->snap->snapFlags;
		int ping = cgs->snap->ping;

		static int snapDelay = 0;
		if (snap < 0.f)
			snapDelay = std::abs(snap);

		Snaps.Add(snap);
		SnapsDelay.Add(snapDelay);
		SnapsFlags.Add(snapFlags);
		Pings.Add(ping);

		Graph.Begin();
		if (ImPlot::BeginPlot("##Ping", Graph.RenderSize))
		{
			ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_Canvas, ImPlotAxisFlags_Canvas);
			ImPlot::SetupAxisLimits(ImAxis_X1, 0, Snaps.Size(), ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1000, ImGuiCond_Always);

			if (ShowSnap)
			{
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(SnapColor));
				ImPlot::PlotShaded("Snap", Snaps.Get(), Snaps.Size(), -INFINITY, 1, 0, 0, Snaps.Offset);
				ImPlot::PlotLine("Snap", Snaps.Get(), Snaps.Size(), 1, 0, 0, Snaps.Offset);
				ImPlot::PopStyleColor();
			}
			if (ShowPing)
			{
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(PingColor));
				ImPlot::PlotShaded("Ping", Pings.Get(), Pings.Size(), -INFINITY, 1, 0, 0, Pings.Offset);
				ImPlot::PlotLine("Ping", Pings.Get(), Pings.Size(), 1, 0, 0, Pings.Offset);
				ImPlot::PopStyleColor();
			}
			ImPlot::EndPlot();
		}
		Graph.End();
	}
}

#include "Lagometer.hpp"

namespace IW3SR::Addons
{
	Lagometer::Lagometer() : Module("sr.player.lagometer", "Lagometer", "Player")
	{
		PingColor = { 1, 1, 1, 1 };
		PacketColor = { 1, 0, 0, 0.4 };
		ShowPing = true;
		ShowPacket = false;

		Graph = Window("Lag graph");
		ShowGraph = true;
	}

	void Lagometer::OnMenu()
	{
		ImGui::Checkbox("Show graph", &ShowGraph);
		if (ShowGraph)
		{
			ImGui::NewLine();
			ImGui::SameLine(35);
			ImGui::Checkbox("Show ping", &ShowPing);
			ImGui::NewLine();
			ImGui::SameLine(35);
			ImGui::Checkbox("Show packets", &ShowPacket);
		}
	}

	void Lagometer::OnRender()
	{
		Ping = cgs->snap->ping;
		PingBuffer.Add(Ping);
		Packet = clc.lastPacketSentTime - clc.lastPacketTime;
		PacketBuffer.Add(Packet);

		if (ShowGraph)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			Graph.Begin(ImGuiWindowFlags_Graph);
			if (ImPlot::BeginPlot("##Ping", Graph.RenderSize))
			{
				ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(PingColor));
				ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_Canvas, ImPlotAxisFlags_Canvas);
				ImPlot::SetupAxisLimits(ImAxis_X1, 0, PingBuffer.Size(), ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 999, ImGuiCond_Always);

				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				if (ShowPing)
				{
					ImPlot::PlotShaded("ping", PingBuffer.Get(), PingBuffer.Size(), -INFINITY, 1, 0, 0, PingBuffer.Offset);
					ImPlot::PlotLine("ping", PingBuffer.Get(), PingBuffer.Size(), 1, 0, 0, PingBuffer.Offset);
				}
				ImPlot::PopStyleColor();

				ImPlot::PushStyleColor(ImPlotCol_Line, static_cast<ImU32>(PacketColor));
				if (ShowPacket)
				{
					ImPlot::PlotShaded("packet", PacketBuffer.Get(), PacketBuffer.Size(), -INFINITY, 1, 0, 0, PacketBuffer.Offset);
					ImPlot::PlotLine("packet", PacketBuffer.Get(), PacketBuffer.Size(), 1, 0, 0, PacketBuffer.Offset);
				}
				ImPlot::PopStyleColor();
				ImPlot::EndPlot();
			}
			Graph.End();
			ImGui::PopStyleVar();
		}
	}
}

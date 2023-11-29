#include "Toolbar.hpp"

namespace IW3SR
{
	Toolbar::Toolbar() : Window("Toolbar")
	{
		SetRect(0, 0, 640, 0);
	}

	void Toolbar::Frame()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });

		Begin(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

		const ImVec2& position = RenderPosition;
		const ImVec2& size = RenderSize;
		const ImVec2 buttonSize = { 30, 30 };

		ImGui::Rainbow(position + ImVec2{ 0, size.y }, position + ImVec2{ size.x, size.y + 2 });
		ImGui::Button(ICON_FA_GAMEPAD, "Modules", &SR->Modules->Menu.Open, buttonSize);
		ImGui::Tooltip("Modules");
		ImGui::SameLine();

		if (Utils::IsDebug())
		{
			if (ImGui::Button(ICON_FA_ROTATE_RIGHT, "Reload", nullptr, buttonSize))
				SR->Modules->Reload();
			ImGui::Tooltip("Reload plugins");
			ImGui::SameLine();
		}
		ImGui::ButtonToggle(ICON_FA_PAINTBRUSH, "Design", &GUI::DesignMode, buttonSize);
		ImGui::Tooltip("Design mode");
		ImGui::SameLine();

		if (Utils::IsDebug())
		{
			static bool debug = false;
			ImGui::Button(ICON_FA_TERMINAL, "Debug", &debug, buttonSize);
			if (debug) ImGui::ShowDebugLogWindow(&debug);
			ImGui::Tooltip("Debug");
			ImGui::SameLine();
		}
		ImGui::Button(ICON_FA_CIRCLE_INFO, "About", nullptr, buttonSize);
		ImGui::Tooltip("About");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_GEAR, "Settings", nullptr, buttonSize);
		ImGui::Tooltip("Settings");

		End();
		ImGui::PopStyleVar(2);
	}
}

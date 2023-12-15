#include "Toolbar.hpp"

namespace IW3SR::UI
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
		ImGui::Button(ICON_FA_GAMEPAD, "Modules", &GUI::Modules.Open, buttonSize);
		ImGui::Tooltip("Modules");
		ImGui::SameLine();

		if (Utils::IsDebug())
		{
			if (ImGui::Button(ICON_FA_ROTATE_RIGHT, "Reload", nullptr, buttonSize))
			{
				SR->Modules->Serialize();
				SR->DLLS->Reload();
			}
			ImGui::Tooltip("Reload plugins");
			ImGui::SameLine();
		}
		ImGui::ButtonToggle(ICON_FA_GRIP, "Design", &GUI::DesignMode, buttonSize);
		ImGui::Tooltip("Design mode");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_PAINTBRUSH, "Themes", &GUI::Themes.Open, buttonSize);
		ImGui::Tooltip("Themes");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_KEYBOARD, "Binds", &GUI::Binds.Open, buttonSize);
		ImGui::Tooltip("Binds");
		ImGui::SameLine();

		if (Utils::IsDebug())
		{
			static bool debug = false;
			ImGui::Button(ICON_FA_TERMINAL, "Debug", &debug, buttonSize);
			ImGui::Tooltip("Debug");
			ImGui::SameLine();
			if (debug) ImGui::ShowDebugLogWindow(&debug);

			ImGui::Button(ICON_FA_MEMORY, "Memory", &GUI::Memory.Open, buttonSize);
			ImGui::Tooltip("Memory");
			ImGui::SameLine();
		}
		ImGui::Button(ICON_FA_CIRCLE_INFO, "About", &GUI::About.Open, buttonSize);
		ImGui::Tooltip("About");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_GEAR, "Settings", &GUI::Settings.Open, buttonSize);
		ImGui::Tooltip("Settings");

		End();
		ImGui::PopStyleVar(2);
	}
}

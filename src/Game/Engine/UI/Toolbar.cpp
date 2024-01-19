#include "Toolbar.hpp"
#include "Engine/Core/Utils/Utils.hpp"

namespace IW3SR::UI
{
	Toolbar::Toolbar() : Window("Toolbar")
	{
		SetRect(0, 0, 640, 0);
	}

	void Toolbar::Render()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });

		Begin(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

		const vec2& position = RenderPosition;
		const vec2& size = RenderSize;
		const vec2 buttonSize = { 30, 30 };

		ImGui::Rainbow(position + vec2{ 0, size.y }, position + vec2{ size.x, size.y + 2 });
		ImGui::Button(ICON_FA_GAMEPAD, "Modules", &GetGUI()->Modules.Open, buttonSize);
		ImGui::Tooltip("Modules");
		ImGui::SameLine();

		if (Utils::IsDebug())
		{
			if (ImGui::Button(ICON_FA_ROTATE_RIGHT, "Reload", nullptr, buttonSize))
				Reload();
			ImGui::Tooltip("Reload plugins");
			ImGui::SameLine();
		}
		ImGui::ButtonToggle(ICON_FA_GRIP, "Design", &GetGUI()->DesignMode, buttonSize);
		ImGui::Tooltip("Design mode");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_PAINTBRUSH, "Themes", &GetGUI()->Themes.Open, buttonSize);
		ImGui::Tooltip("Themes");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_KEYBOARD, "Binds", &GetGUI()->Binds.Open, buttonSize);
		ImGui::Tooltip("Binds");
		ImGui::SameLine();

		if (Utils::IsDebug())
		{
			ImGui::Button(ICON_FA_TERMINAL, "Debug", &IsDebug, buttonSize);
			ImGui::Tooltip("Debug");
			ImGui::SameLine();

			if (IsDebug)
			{
				ImGui::ShowDebugLogWindow(&IsDebug);
				ImGui::ShowStackToolWindow(&IsDebug);
			}
			ImGui::Button(ICON_FA_MEMORY, "Memory", &GetGUI()->Memory.Open, buttonSize);
			ImGui::Tooltip("Memory");
			ImGui::SameLine();
		}
		ImGui::Button(ICON_FA_CIRCLE_INFO, "About", &GetGUI()->About.Open, buttonSize);
		ImGui::Tooltip("About");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_GEAR, "Settings", &GetGUI()->Settings.Open, buttonSize);
		ImGui::Tooltip("Settings");

		End();
		ImGui::PopStyleVar(2);
	}

	void Toolbar::Reload()
	{
		if (IsReloading) return;
		IsReloading = true;

		GetRenderer()->Modules->Serialize();
		GetRenderer()->Features->Serialize();

		Plugins::Shutdown();
		std::thread([this] { Compile(); }).detach();
	}

	void Toolbar::Compile()
	{
		if (std::filesystem::exists(CMAKE_BINARY_DIR))
		{
			constexpr auto command = R"(cd "{}" && cmake --build . --config Debug --target Install)";
			system(std::format(command, CMAKE_BINARY_DIR).c_str());
		}
		Plugins::Initialize();

		GetRenderer()->Modules->Deserialize();
		GetRenderer()->Features->Deserialize();

		IsReloading = false;
	}
}

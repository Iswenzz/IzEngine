#include "Core/Console/Console.hpp"
#include "Core/System/Crash.hpp"
#include "Core/System/Environment.hpp"
#include "Core/System/System.hpp"
#include "Core/System/Window.hpp"

#include "DX9/Common.hpp"
#include "ImGUI/Common.hpp"
#include "Renderer/Common.hpp"

class Toolbar : public Frame
{
public:
	Toolbar() : Frame("Toolbar")
	{
		Open = true;
		SetRectAlignment(HORIZONTAL_FULLSCREEN, VERTICAL_FULLSCREEN);
		SetFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
	}

	void OnRender() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
		SetRect(0, 0, 640, 16);

		Begin();

		static bool isDebug = false;
		const vec2& position = RenderPosition;
		const vec2& size = RenderSize;
		const vec2 buttonSize = { size.y, size.y };

		ImDrawList* draw = ImGui::GetBackgroundDrawList();
		draw->AddRectFilled(position, vec2(position + size), ImColor(ImGui::GetStyleColorVec4(ImGuiCol_FrameBg)));

		ImGui::Rainbow(position + vec2{ 0, size.y }, position + vec2{ size.x, size.y + 2 });
		ImGui::ButtonToggle(ICON_FA_GRIP, "Design", &UI::DesignMode, buttonSize);
		ImGui::Tooltip("Design mode");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_PAINTBRUSH, "Themes", &UI::Frames["Themes"]->Open, buttonSize);
		ImGui::Tooltip("Themes");
		ImGui::SameLine();
		ImGui::Button(ICON_FA_TERMINAL, "Debug", &isDebug, buttonSize);
		ImGui::Tooltip("Debug");
		ImGui::SameLine();

		if (isDebug)
		{
			ImGui::ShowDebugLogWindow(&isDebug);
			ImGui::ShowStackToolWindow(&isDebug);
		}
		ImGui::Button(ICON_FA_MEMORY, "Memory", &UI::Frames["Memory"]->Open, buttonSize);
		ImGui::Tooltip("Memory");
		ImGui::SameLine();

		End();
		ImGui::PopStyleVar(2);
	}
};

void Application::Start()
{
	Crash::Setup();
	Environment::Local();
	Console::Initialize("IzEngine");
	Window::Initialize("IzEngine");

	Device::Initialize();
	Renderer::Initialize();

	UI::Open = true;
	UI::Add<Toolbar>();

	while (Window::Open)
	{
		Window::Frame();
		Renderer::Frame();
	}
	Shutdown();
}

void Application::Shutdown()
{
	Renderer::Shutdown();
	Device::Shutdown();
	Window::Shutdown();
	Console::Shutdown();
}

void Application::Dispatch(Event& event)
{
	UI::Dispatch(event);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Application::Start();
	return 0;
}

#include "GUI.hpp"
#include "Game/Game.hpp"

namespace IW3SR::Game
{
	GUI::GUI() : UI(UI::Get())
	{
		Toolbar = UC::Toolbar();
		About = UC::About();
		Binds = UC::Binds();

		Environment::Deserialize("GUI", *this);
	}

	GUI::~GUI()
	{
		Environment::Serialize("GUI", *this);
	}

	void GUI::Initialize()
	{
		if (UI.Active) return;

		UI.Initialize();
		ImGui_ImplWin32_Init(Sys::MainWindow);
		ImGui_ImplDX9_Init(dx->device);

		Plugins::SetRenderer();
	}

	void GUI::Release()
	{
		if (!UI.Active) return;

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		UI.Release();
	}

	void GUI::Begin()
	{
		if (!UI.Active) return;
		
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		UI.Begin();

		Render();
	}

	void GUI::End()
	{
		if (!UI.Active) return;

		UI.End();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		KeyListener::Reset();
	}

	void GUI::Render()
	{
		if (!UI.Open) return;

		Toolbar.Render();
		Binds.Render();
		About.Render();

		UI.Memory.Render();
		UI.Modules.Render();
		UI.Settings.Render();
		UI.Themes.Render();
	}
}

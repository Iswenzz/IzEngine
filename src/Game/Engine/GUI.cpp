#include "GUI.hpp"
#include "Game/Sys/Sys.hpp"

#include "Engine/Backends/DX9/Device.hpp"

namespace IW3SR::Game
{
	GUI::GUI() : UI(UI::Get())
	{
		Toolbar = UC::Toolbar();
		About = UC::About();
		Binds = UC::Binds();

		Environment::Deserialize("GUI", *this);
	}

	void GUI::Initialize()
	{
		UI.Initialize();
	}

	void GUI::Release()
	{
		UI.Release();
		Environment::Serialize("GUI", *this);
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

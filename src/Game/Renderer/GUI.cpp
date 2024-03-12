#include "GUI.hpp"

namespace IW3SR::Game
{
	GUI::GUI() : UI(UI::Get())
	{
		KeyOpen = KeyListener(VK_F10);

		About = UC::About();
		Binds = UC::Binds();
		Modules = UC::Modules();
		Settings = UC::Settings();
		Toolbar = UC::Toolbar();
	}

	void GUI::Initialize()
	{
		Environment::Deserialize("GUI", *this);
		UI.Initialize();
	}

	void GUI::Release()
	{
		UI.Release();
		Environment::Serialize("GUI", *this);
	}

	void GUI::Render()
	{
		if (KeyOpen.IsPressed())
			UI.Open = !UI.Open;

		if (!UI.Open)
			return;

		Toolbar.Render();
		Binds.Render();
		Modules.Render();
		Settings.Render();
		About.Render();

		UI.Memory.Render();
		UI.Themes.Render();
	}
}

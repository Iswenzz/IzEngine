#include "Engine/Common.hpp"

#include "Engine/Core/Browser/Browser.hpp"

void Application::Start()
{
	Crash::Setup();
	Environment::Binary();

	Console::Initialize("IzEngine");
	Window::Initialize("IzEngine");
	Renderer::Initialize(RendererBackend::DX9);

	UI::OpenMenu();
	UI::OpenWindow("Memory");

	while (Window::Frame())
		Renderer::Frame();

	Shutdown();
}

void Application::Shutdown()
{
	Renderer::Shutdown();
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

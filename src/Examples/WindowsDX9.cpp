#include "Engine/Core/Application/Application.hpp"
#include "Engine/Core/Console/Console.hpp"
#include "Engine/Core/System/Crash.hpp"
#include "Engine/Core/System/Environment.hpp"
#include "Engine/Core/System/Window.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"
#include "Engine/Renderer/Common.hpp"

void Application::Start()
{
	Crash::Setup();
	Environment::Binary();

	Console::Initialize("IzEngine");
	Window::Initialize("IzEngine");
	Renderer::Initialize(RendererBackend::DX9);

	UI::OpenMenu();
	UI::OpenWindow("Memory");

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

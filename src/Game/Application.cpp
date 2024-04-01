#include "Core/Common.hpp"

#include "Renderer/Modules/Modules.hpp"
#include "Renderer/Settings/Settings.hpp"

#include "Player/Player.hpp"
#include "System/Console.hpp"
#include "System/Patch.hpp"

namespace IW3SR::Engine
{
	void Application::Start()
	{
		Environment::Initialize();
		Environment::Load();

		Patch::Initialize();
		Console::Initialize();
		Player::Initialize();
		Plugins::Initialize();
	}

	void Application::Shutdown()
	{
		Environment::Save();

		Console::Release();
		Plugins::Shutdown();
		Patch::Shutdown();
	}

	void Application::Dispatch(Event& event)
	{
		Plugins::Dispatch(event);
		Modules::Dispatch(event);
		Settings::Dispatch(event);
	}
}

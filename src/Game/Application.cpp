#include "Core/Base.hpp"
#include "Core/System/Environment.hpp"
#include "Core/System/Plugins.hpp"

#include "Renderer/Modules/Modules.hpp"
#include "Renderer/Settings/Settings.hpp"

#include "Player/Player.hpp"
#include "System/Console.hpp"
#include "System/Patch.hpp"

namespace IW3SR
{
	void Application::Start()
	{
		Environment::Initialize();
		Environment::Load();

		Patch::Initialize();
		GConsole::Initialize();
		Player::Initialize();
		Plugins::Initialize();
	}

	void Application::Shutdown()
	{
		Environment::Save();

		GConsole::Shutdown();
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

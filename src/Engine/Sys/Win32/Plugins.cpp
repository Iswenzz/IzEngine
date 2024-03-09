#include "Plugins.hpp"

namespace IW3SR::Engine
{
	void Plugins::Initialize()
	{
		if (!std::filesystem::is_directory(Environment::PluginsDirectory))
			return;
		
		for (const auto& entry : std::filesystem::directory_iterator(Environment::PluginsDirectory))
		{
			if (entry.path().extension() != ".dll")
				continue;

			auto mod = std::make_unique<Plugin>(entry.path().string());
			if (mod->Loaded)
				Modules.insert({ entry.path().filename().string(), std::move(mod) });
		}
	}

	void Plugins::Shutdown()
	{
		Modules.clear();
	}

	void Plugins::Dispatch(Event& event)
	{
		for (const auto& [_, mod] : Modules)
			mod->OnEvent(event);
	}
}

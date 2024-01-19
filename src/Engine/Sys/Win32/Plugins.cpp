#include "Plugins.hpp"

namespace IW3SR
{
	void Plugins::Initialize()
	{
		if (!std::filesystem::is_directory(Environment::PluginsDirectory))
			return;

		for (const auto& entry : std::filesystem::directory_iterator(Environment::PluginsDirectory))
		{
			if (entry.path().extension() == ".dll")
				Modules.insert({ entry.path().filename().string(), std::make_unique<Plugin>(entry.path().string()) });
		}
	}

	void Plugins::SetRenderer()
	{
		for (const auto& [_, plugin] : Modules)
			plugin->SetRenderer();
	}

	void Plugins::Shutdown()
	{
		Modules.clear();
	}
}

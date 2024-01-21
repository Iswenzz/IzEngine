#include "Plugin.hpp"
#include <stdexcept>

namespace IW3SR::Engine
{
	Plugin::Plugin(std::string filePath)
	{
		FilePath = filePath;
		Instance = LoadLibrary(FilePath.c_str());

		if (!Instance)
			throw std::runtime_error("Couldn't load plugin.");

		CallbackInitialize = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "Initialize"));
		CallbackRenderer = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "Renderer"));
		CallbackShutdown = reinterpret_cast<uintptr_t>(GetProcAddress(Instance, "Shutdown"));

		if (!CallbackInitialize)
			throw std::runtime_error("Plugin is missing Initialize function.");

		SetRenderer();
		CallbackInitialize(this);
	}

	Plugin::~Plugin()
	{
		if (CallbackShutdown)
			CallbackShutdown();

		FreeLibrary(Instance);
		Instance = nullptr;
	}

	void Plugin::SetRenderer()
	{
		if (CallbackRenderer)
			CallbackRenderer();
	}

	void Plugin::SetInfos(const std::string& id, const std::string& name)
	{
		ID = id;
		Name = name;
	}
}

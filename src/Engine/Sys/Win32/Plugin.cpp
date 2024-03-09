#include "Plugin.hpp"
#include <stdexcept>

namespace IW3SR::Engine
{
	Plugin::Plugin(std::string filePath)
	{
		FilePath = filePath;
		Instance = LoadLibrary(FilePath.c_str());

		if (!Instance)
		{
			Log::WriteLine(Channel::Error, "Invalid plugin {}", filePath);
			return;
		}
		CallbackInitialize < Signature(GetProcAddress(Instance, "Initialize"));
		CallbackRenderer < Signature(GetProcAddress(Instance, "Renderer"));
		CallbackShutdown < Signature(GetProcAddress(Instance, "Shutdown"));

		Loaded = CallbackInitialize;

		if (CallbackInitialize)
			CallbackInitialize(this);
	}

	Plugin::~Plugin()
	{
		if (CallbackShutdown)
			CallbackShutdown();

		FreeLibrary(Instance);
		Instance = nullptr;
	}

	void Plugin::SetInfos(const std::string& id, const std::string& name)
	{
		ID = id;
		Name = name;
	}

	void Plugin::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		if (CallbackRenderer)
			dispatcher.Dispatch<EventPluginRenderer>(CallbackRenderer.Func);
	}
}

#include "Windows/Base.hpp"

#include "Core/Memory/Signature.hpp"
#include "Core/System/Plugin.hpp"

namespace IzEngine
{
	const char* Plugin::Extension = ".dll";

	Plugin::Plugin(std::string filePath)
	{
		FilePath = filePath;
		HINSTANCE instance = LoadLibrary(FilePath.c_str());

		if (!instance)
		{
			Log::WriteLine(Channel::Error, "Invalid plugin {}", filePath);
			return;
		}
		CallbackInitialize < uintptr_t(GetProcAddress(instance, "Initialize"));
		CallbackGame < uintptr_t(GetProcAddress(instance, "Game"));
		CallbackShutdown < uintptr_t(GetProcAddress(instance, "Shutdown"));

		Instance = instance;
		Loaded = CallbackInitialize;

		if (CallbackInitialize)
			CallbackInitialize(this);
	}

	Plugin::~Plugin()
	{
		if (CallbackShutdown)
			CallbackShutdown();

		if (Instance)
		{
			FreeLibrary(reinterpret_cast<HINSTANCE>(Instance));
			Instance = nullptr;
		}
	}

	void Plugin::SetInfos(const std::string& id, const std::string& name)
	{
		ID = id;
		Name = name;
	}

	void Plugin::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		if (CallbackGame)
			dispatcher.Dispatch<EventPluginGame>(CallbackGame.Func);
	}
}

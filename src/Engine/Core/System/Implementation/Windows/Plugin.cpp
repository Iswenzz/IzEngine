#include "Windows/Base.hpp"

#include "Core/Console/Console.hpp"
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
		CallbackInfo < uintptr_t(GetProcAddress(instance, "Info"));
		CallbackInitialize < uintptr_t(GetProcAddress(instance, "Initialize"));
		CallbackShutdown < uintptr_t(GetProcAddress(instance, "Shutdown"));

		Instance = instance;
		Loaded = CallbackInfo;

		if (CallbackInfo)
			CallbackInfo(this);
		if (CallbackInitialize)
			CallbackInitialize();
	}

	Plugin::~Plugin()
	{
		if (Console::Terminated)
			return;

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
}

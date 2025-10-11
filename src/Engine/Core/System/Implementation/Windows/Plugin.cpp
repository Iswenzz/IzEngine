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
		HMODULE mod = LoadLibrary(FilePath.c_str());

		if (!mod)
		{
			Log::WriteLine(Channel::Error, "Invalid plugin {}", filePath);
			return;
		}
		CallbackInitialize < uintptr_t(GetProcAddress(mod, "Initialize"));
		CallbackShutdown < uintptr_t(GetProcAddress(mod, "Shutdown"));

		if (!CallbackInitialize || !CallbackShutdown)
		{
			Log::WriteLine(Channel::Error, "Invalid plugin {}", filePath);
			FreeLibrary(mod);
			return;
		}
		Instance = mod;
		Loaded = true;
	}

	Plugin::~Plugin()
	{
		if (!Instance)
			return;

		Shutdown();
		FreeLibrary(reinterpret_cast<HMODULE>(Instance));
		Instance = nullptr;
	}

	void Plugin::Initialize()
	{
		if (Active)
			return;

		CallbackInitialize();
		Active = true;
	}

	void Plugin::Shutdown()
	{
		if (!Active)
			return;

		CallbackShutdown();
		Active = false;
	}
}

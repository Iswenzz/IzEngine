#pragma once
#include "Engine/Core/Memory/Function.hpp"

namespace IzEngine
{
	class Plugin : public IObject
	{
	public:
		void* Instance = nullptr;
		std::string FilePath;
		bool Loaded = false;
		bool Active = false;

		static const char* Extension;

		Plugin() = default;
		Plugin(std::string filePath);
		virtual ~Plugin();

		void Initialize();
		void Shutdown();

	private:
		Function<void()> CallbackInitialize;
		Function<void()> CallbackShutdown;
	};

	class Plugins
	{
	public:
		static inline std::unordered_map<std::string, Scope<Plugin>> Modules;
		static inline bool IsReloading = false;

		static void Load();
		static void Free();

		static void Initialize();
		static void Shutdown();
	};
}

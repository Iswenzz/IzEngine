#pragma once
#include "Engine/Sys/Win32.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Plugin class.
	/// </summary>
	class API Plugin
	{
	public:
		std::string ID;
		std::string Name;
		std::string FilePath;
		bool Loaded = false;

		HINSTANCE Instance = nullptr;
		Function<void(Plugin* plugin)> CallbackInitialize;
		Function<void()> CallbackRenderer;
		Function<void()> CallbackShutdown;

		/// <summary>
		/// Create a new plugin.
		/// </summary>
		Plugin() = default;

		/// <summary>
		/// Create a new plugin.
		/// </summary>
		/// <param name="filePath">The plugin file path.</param>
		Plugin(std::string filePath);

		/// <summary>
		/// Release the plugin.
		/// </summary>
		~Plugin();

		/// <summary>
		/// Set plugin infos.
		/// </summary>
		/// <param name="id">The ID.</param>
		/// <param name="name">The name.</param>
		void SetInfos(const std::string& id, const std::string& name);
	};
}

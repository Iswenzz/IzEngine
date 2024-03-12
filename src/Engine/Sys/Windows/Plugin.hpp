#pragma once
#include "Windows/Base.hpp"
#include "Core/Common.hpp"

#include "Sys/Memory/Function.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Plugin class.
	/// </summary>
	class API Plugin : public IObject
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

		/// <summary>
		/// Event dispatch.
		/// </summary>
		/// <param name="event">The event.</param>
		void OnEvent(Event& event) override;
	};
}

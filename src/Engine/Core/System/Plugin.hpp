#pragma once
#include "Core/Base.hpp"
#include "Core/Memory/Function.hpp"

namespace IzEngine
{
	/// <summary>
	/// Plugin class.
	/// </summary>
	class Plugin : public IObject
	{
	public:
		static const char* Extension;

		void* Instance = nullptr;
		std::string FilePath;
		bool Loaded = false;
		bool Active = false;

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
		virtual ~Plugin();

		/// <summary>
		/// Initialize the plugin.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Shutdown the plugin.
		/// </summary>
		void Shutdown();

	private:
		Function<void()> CallbackInitialize;
		Function<void()> CallbackShutdown;

	};
}

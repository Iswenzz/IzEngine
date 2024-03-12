#pragma once
#include "Plugin.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Plugins class.
	/// </summary>
	class Plugins
	{
	public:
		static inline std::map<std::string, Scope<Plugin>> Modules;
		static inline bool IsReloading = false;

		/// <summary>
		/// Initialize the plugins.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the plugins.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Dispatch event.
		/// </summary>
		/// <param name="event">The event.</param>
		static void Dispatch(Event& event);
	};
}

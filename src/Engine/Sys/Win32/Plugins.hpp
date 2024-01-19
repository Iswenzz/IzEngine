#pragma once
#include "Plugin.hpp"

namespace IW3SR
{
	/// <summary>
	/// Plugins class.
	/// </summary>
	class Plugins
	{
	public:
		static inline std::map<std::string, std::unique_ptr<Plugin>> Modules;
		static inline bool IsReloading = false;

		/// <summary>
		/// Initialize the plugins.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Set the plugins renderer.
		/// </summary>
		static void SetRenderer();

		/// <summary>
		/// Shutdown the plugins.
		/// </summary>
		static void Shutdown();
	};
}

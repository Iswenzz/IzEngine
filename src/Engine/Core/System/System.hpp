#pragma once
#include "Engine/Base.hpp"

namespace IzEngine
{
	/// <summary>
	/// Engine system.
	/// </summary>
	class API System
	{
	public:
		static inline std::map<std::string, uintptr_t> Processes;
		static inline std::map<std::string, uintptr_t> Modules;

		/// <summary>
		/// Map processes.
		/// </summary>
		static void MapProcesses();

		/// <summary>
		/// Map modules.
		/// </summary>
		/// <param name="process">The process.</param>
		static void MapModules(uintptr_t process);

		/// <summary>
		/// Execute shell command.
		/// </summary>
		/// <param name="command">The command.</param>
		static void Shell(const std::string& command);

		/// <summary>
		/// Is debug mode.
		/// </summary>
		/// <returns></returns>
		static bool IsDebug();
	};
}

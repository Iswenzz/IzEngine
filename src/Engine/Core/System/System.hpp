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
		static inline void* MainWindow = nullptr;

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

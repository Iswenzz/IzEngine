#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API System
	{
	public:
		static std::map<std::string, uintptr_t> MapProcesses();
		static std::map<std::string, uintptr_t> MapModules(uintptr_t process);
		static void Shell(const std::string& command);
		static bool IsDebug();
	};
}

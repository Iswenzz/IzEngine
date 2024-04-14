#include "Windows/Base.hpp"

#include <shellapi.h>
#include "Core/System/System.hpp"

namespace IW3SR::Engine
{
	void System::Shell(const std::string& command)
	{
		ShellExecute(nullptr, "open", command.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	}

	bool System::IsDebug()
	{
#ifdef _DEBUG
		return true;
#else
		return false;
#endif
	}
}

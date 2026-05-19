#include "Base.hpp"

#include "Engine/Core/IO/VFS.hpp"
#include "Engine/Core/System/Environment.hpp"

namespace IzEngine
{
	void Environment::Binary()
	{
		TCHAR buffer[MAX_PATH];
		GetModuleFileName(nullptr, buffer, MAX_PATH);
		Directories.insert({ Directory::Base, std::filesystem::path(buffer).parent_path() });
		Initialize();
	}

	void Environment::Local()
	{
		Directories.insert({ Directory::Base, std::filesystem::path(getenv("LOCALAPPDATA")) });
		Initialize();
	}
}

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
		const char* localAppData = getenv("LOCALAPPDATA");
		if (!localAppData)
		{
			Binary();
			return;
		}
		Directories.insert({ Directory::Base, std::filesystem::path(localAppData) });
		Initialize();
	}
}

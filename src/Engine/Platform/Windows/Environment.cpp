#include "Base.hpp"

#include "Engine/Core/IO/VFS.hpp"
#include "Engine/Core/System/Environment.hpp"

namespace IzEngine
{
	std::filesystem::path Environment::Executable()
	{
		wchar_t buffer[MAX_PATH] = {};
		GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		return buffer;
	}

	void Environment::Binary()
	{
		Directories.insert({ Directory::Base, Executable().parent_path() });
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

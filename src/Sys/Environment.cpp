#include "Environment.hpp"
#include "Game/Definitions.hpp"

#include <Windows.h>

namespace IW3SR
{
	void Environment::Build()
	{
		BuildProcessPath();
		IW3SRDirectory = BaseDirectory / "iw3sr";
		ModulesDirectory = IW3SRDirectory / "modules";
		PluginsDirectory = IW3SRDirectory / "plugins";
		RessourcesDirectory = IW3SRDirectory / "ressources";
		FontsDirectory = RessourcesDirectory / "fonts";
		ImagesDirectory = RessourcesDirectory / "images";
	}

	void Environment::BuildProcessPath()
	{
		TCHAR buffer[MAX_PATH];
		DWORD bufferLength = GetModuleFileName(NULL, buffer, MAX_PATH);
		BaseDirectory = std::filesystem::path(buffer).parent_path();
	}
}

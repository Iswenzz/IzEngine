#include "Windows/Base.hpp"

#include "Core/System/Environment.hpp"

#include <TlHelp32.h>
#include <fstream>

namespace IzEngine
{
	void Environment::Initialize(const std::string& install)
	{
		Build();
		BuildModulesList();

		InstallDirectory = BaseDirectory / install;
		PluginsDirectory = InstallDirectory / "plugins";
		ResourcesDirectory = InstallDirectory / "resources";
		FontsDirectory = ResourcesDirectory / "fonts";
		ImagesDirectory = ResourcesDirectory / "images";
	}

	void Environment::Build()
	{
		TCHAR buffer[MAX_PATH];
		GetModuleFileName(nullptr, buffer, MAX_PATH);
		BaseDirectory = std::filesystem::path(buffer).parent_path();
	}

	void Environment::BuildModulesList()
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return;

		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapshot, &moduleEntry))
		{
			do
			{
				Modules.push_back(moduleEntry.szModule);
			} while (Module32Next(hSnapshot, &moduleEntry));
		}
		CloseHandle(hSnapshot);
	}

	void Environment::Save()
	{
		std::ofstream file(Environment::InstallDirectory / "application.json");
		file << Settings.dump(4);
		file.close();
	}

	void Environment::Load()
	{
		std::ifstream file(Environment::InstallDirectory / "application.json");
		if (file.peek() != std::ifstream::traits_type::eof())
			Settings = nlohmann::json::parse(file);
	}
}

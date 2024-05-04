#include "Windows/Base.hpp"

#include "Core/System/Environment.hpp"

#include <TlHelp32.h>
#include <fstream>

namespace IzEngine
{
	void Environment::Binary()
	{
		TCHAR buffer[MAX_PATH];
		GetModuleFileName(nullptr, buffer, MAX_PATH);
		BaseDirectory = std::filesystem::path(buffer).parent_path();
		Initialize();
	}

	void Environment::Local() 
	{
		BaseDirectory = std::filesystem::path(getenv("LOCALAPPDATA"));
		Initialize();
	}

	void Environment::Initialize()
	{
		BuildModules();

		AppDirectory = BaseDirectory / APPLICATION_ID;
		PluginsDirectory = AppDirectory / "plugins";
		ResourcesDirectory = AppDirectory / "resources";
		ReportsDirectory = AppDirectory / "reports";
		FontsDirectory = ResourcesDirectory / "fonts";
		ImagesDirectory = ResourcesDirectory / "images";

		std::filesystem::create_directory(AppDirectory);
		std::filesystem::create_directory(PluginsDirectory);
		std::filesystem::create_directory(ResourcesDirectory);
		std::filesystem::create_directory(ReportsDirectory);
		std::filesystem::create_directory(FontsDirectory);
		std::filesystem::create_directory(ImagesDirectory);
	}

	void Environment::BuildModules()
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
		std::ofstream file(Environment::AppDirectory / "application.json");
		file << Settings.dump(4);
		file.close();
	}

	void Environment::Load()
	{
		std::ifstream file(Environment::AppDirectory / "application.json");
		if (file.peek() != std::ifstream::traits_type::eof())
			Settings = nlohmann::json::parse(file);
	}
}

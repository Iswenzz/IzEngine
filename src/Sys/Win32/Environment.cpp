#include "Environment.hpp"

namespace IW3SR
{
	void Environment::Initialize()
	{
		Build();
		BuildModulesList();

		IW3SRDirectory = BaseDirectory / "iw3sr";
		PluginsDirectory = IW3SRDirectory / "plugins";
		ResourcesDirectory = IW3SRDirectory / "resources";
		FontsDirectory = ResourcesDirectory / "fonts";
		ImagesDirectory = ResourcesDirectory / "images";
	}

	void Environment::Build()
	{
		TCHAR buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
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
			}
			while (Module32Next(hSnapshot, &moduleEntry));
		}
		CloseHandle(hSnapshot);
	}

	void Environment::Save()
	{
		std::ofstream file(Environment::IW3SRDirectory / "settings.json");
		file << Settings.dump(4);
		file.close();
	}

	void Environment::Load()
	{
		std::fstream file(Environment::IW3SRDirectory / "settings.json", std::ios::in | std::ios::out | std::ios::app);
		if (file.peek() != std::ifstream::traits_type::eof())
			Settings = nlohmann::json::parse(file);
	}
}

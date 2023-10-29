#include "Environment.hpp"
#include "Game/Definitions.hpp"

#include <Windows.h>

namespace IW3SR
{
	void Environment::Load()
	{
		TCHAR buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		BaseDirectory = std::filesystem::path(buffer).parent_path();

		IW3SRDirectory = BaseDirectory / "iw3sr";
		PluginsDirectory = IW3SRDirectory / "plugins";
		ResourcesDirectory = IW3SRDirectory / "resources";
		FontsDirectory = ResourcesDirectory / "fonts";
		ImagesDirectory = ResourcesDirectory / "images";

		Deserialize();
	}

	void Environment::Save()
	{
		std::ofstream file(Environment::IW3SRDirectory / "settings.json");
		file << Settings.dump(4);
		file.close();
	}

	void Environment::Deserialize()
	{
		std::fstream file(Environment::IW3SRDirectory / "settings.json", std::ios::in | std::ios::out | std::ios::app);
		if (file.peek() != std::ifstream::traits_type::eof())
			Settings = nlohmann::json::parse(file);
	}
}

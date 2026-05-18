#include "Environment.hpp"

#include "Engine/Core/IO/VFS.hpp"

namespace IzEngine
{
	void Environment::Initialize()
	{
		Directories.insert({ Directory::App, Directories[Directory::Base] / APPLICATION_ID });
		Directories.insert({ Directory::Bin, Directories[Directory::App] / "Bin" });
		Directories.insert({ Directory::Configs, Directories[Directory::App] / "Configs" });
		Directories.insert({ Directory::Plugins, Directories[Directory::App] / "Plugins" });
		Directories.insert({ Directory::Resources, Directories[Directory::App] / "Resources" });
		Directories.insert({ Directory::Reports, Directories[Directory::App] / "Reports" });

		for (const auto& [_, path] : Directories)
			std::filesystem::create_directory(path);

		VFS::Index(Directories[Directory::Resources].string(), ".zip");

		Initialized = true;
	}

	void Environment::Load(nlohmann::json& json, const std::string& filename)
	{
		IZ_ASSERT(Environment::Initialized, "Environment not initialized.");

		std::ifstream file(Path(Directory::Configs) / filename);
		if (file.is_open() && file.peek() != std::ifstream::traits_type::eof())
			json = nlohmann::json::parse(file);
	}

	void Environment::Save(const nlohmann::json& json, const std::string& filename)
	{
		IZ_ASSERT(Environment::Initialized, "Environment not initialized.");

		std::ofstream file(Path(Directory::Configs) / filename);
		file << json.dump(4);
	}

	const std::filesystem::path& Environment::Path(Directory directory)
	{
		IZ_ASSERT(Environment::Initialized, "Environment not initialized.");

		return Directories[directory];
	}
}

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

		std::error_code ec;
		for (const auto& [_, path] : Directories)
			std::filesystem::create_directories(path, ec);

		VFS::Index(Directories[Directory::Resources].string(), ".zip");

		Initialized = true;
	}

	void Environment::Load(nlohmann::json& json, const std::string& filename)
	{
		IZ_ASSERT(Environment::Initialized, "Environment not initialized.");

		std::ifstream file(Path(Directory::Configs) / filename);
		if (file.is_open() && file.peek() != std::ifstream::traits_type::eof())
		{
			nlohmann::json parsed = nlohmann::json::parse(file, nullptr, false);
			if (parsed.is_discarded())
				Log::WriteLine(Channel::Warning, "Invalid config file, using defaults: {}", filename);
			else
				json = std::move(parsed);
		}
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

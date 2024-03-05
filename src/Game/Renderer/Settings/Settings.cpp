#include "Settings.hpp"

namespace IW3SR::Game
{
	void Settings::Initialize()
	{
		Deserialize();
	}

	void Settings::Release()
	{
		Serialize();
		Entries.clear();
	}

	void Settings::Remove(const std::string& id)
	{
		auto& entries = Get().Entries;
		if (auto it = entries.find(id); it != entries.end())
			entries.erase(it);
	}

	void Settings::Deserialize()
	{
		std::ifstream file(Environment::IW3SRDirectory / "settings.json");

		if (file.peek() != std::ifstream::traits_type::eof())
			Serialized = nlohmann::json::parse(file);

		for (const auto& [_, entry] : Entries)
		{
			entry->Initialize();
			entry->Deserialize(Serialized[entry->ID]);
		}
	}

	void Settings::Serialize()
	{
		for (const auto& [_, entry] : Entries)
		{
			entry->Serialize(Serialized[entry->ID]);
			entry->Release();
		}
		std::ofstream file(Environment::IW3SRDirectory / "settings.json");
		file << Serialized.dump(4);
		file.close();
	}
}

#include "Settings.hpp"
#include <fstream>

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
		if (auto it = Entries.find(id); it != Entries.end())
			Entries.erase(it);
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

	void Settings::Dispatch(Event& event)
	{
		for (const auto& [_, entry] : Entries)
			entry->OnEvent(event);
	}
}

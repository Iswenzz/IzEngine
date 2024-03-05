#include "Modules.hpp"

namespace IW3SR::Game
{
	void Modules::Initialize()
	{
		Deserialize();
	}

	void Modules::Release()
	{
		Serialize();
		Entries.clear();
	}

	void Modules::Enable(const std::string& id)
	{
		const auto& entry = Entries[id];
		entry->Initialize();
		entry->Deserialize(Serialized[entry->ID]);
		entry->IsEnabled = true;
	}

	void Modules::Disable(const std::string& id)
	{
		const auto& entry = Entries[id];
		entry->IsEnabled = false;
		entry->Serialize(Serialized[entry->ID]);
		entry->Release();
	}

	void Modules::Remove(const std::string& id)
	{
		auto& entries = Get().Entries;
		if (auto it = entries.find(id); it != entries.end())
			entries.erase(it);
	}

	void Modules::Deserialize()
	{
		std::ifstream file(Environment::IW3SRDirectory / "modules.json");

		if (file.peek() != std::ifstream::traits_type::eof())
			Serialized = nlohmann::json::parse(file);

		for (const auto& [_, entry] : Entries)
		{
			entry->Initialize();
			entry->Deserialize(Serialized[entry->ID]);
		}
	}

	void Modules::Serialize()
	{
		for (const auto& [_, entry] : Entries)
		{
			entry->Serialize(Serialized[entry->ID]);
			entry->Release();
		}
		std::ofstream file(Environment::IW3SRDirectory / "modules.json");
		file << Serialized.dump(4);
		file.close();
	}
}

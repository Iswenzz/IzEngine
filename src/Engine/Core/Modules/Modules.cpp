#include "Modules.hpp"

namespace IW3SR
{
	void Modules::Initialize()
	{
		for (const auto& [_, entry] : Entries)
			entry->Initialize();
		Deserialize();
	}

	void Modules::Enable(const std::string& id)
	{
		Entries[id]->IsEnabled = true;
	}

	void Modules::Disable(const std::string& id)
	{
		Entries[id]->IsEnabled = false;
	}

	void Modules::Remove(const std::string& id)
	{
		auto it = Entries.find(id);
		if (it != Entries.end())
			Entries.erase(it);
	}

	void Modules::Deserialize()
	{
		std::fstream file(Environment::IW3SRDirectory / "modules.json", std::ios::in | std::ios::out | std::ios::app);

		if (file.peek() != std::ifstream::traits_type::eof())
			Serialized = nlohmann::json::parse(file);

		for (const auto& [_, entry] : Entries)
			entry->Deserialize(Serialized[entry->ID]);
	}

	void Modules::Serialize()
	{
		for (const auto& [_, entry] : Entries)
			entry->Serialize(Serialized[entry->ID]);

		std::ofstream file(Environment::IW3SRDirectory / "modules.json");
		file << Serialized.dump(4);
		file.close();
	}

	void Modules::Shutdown()
	{
		Serialize();
		for (const auto& [_, entry] : Entries)
			entry->Release();
	}
}

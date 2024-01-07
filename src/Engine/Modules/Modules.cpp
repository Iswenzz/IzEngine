#include "Modules.hpp"

namespace IW3SR
{
	Modules::Modules()
	{
		Deserialize();
	}

	Modules::~Modules()
	{
		Serialize();
	}

	void Modules::Enable(const std::string& id)
	{
		auto& entry = Entries[id];
		entry->IsEnabled = true;
		entry->Initialize();
	}

	void Modules::Disable(const std::string& id)
	{
		auto& entry = Entries[id];
		entry->IsEnabled = false;
		entry->Shutdown();
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
	}

	void Modules::Serialize()
	{
		for (const auto& [_, entry] : Entries)
			entry->Serialize(Serialized[entry->ID]);

		std::ofstream file(Environment::IW3SRDirectory / "modules.json");
		file << Serialized.dump(4);
		file.close();
	}
}

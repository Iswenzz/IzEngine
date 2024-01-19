#include "Features.hpp"

namespace IW3SR
{
	void Features::Remove(const std::string& id)
	{
		auto it = Entries.find(id);
		if (it != Entries.end())
			Entries.erase(it);
	}

	void Features::Deserialize()
	{
		std::fstream file(Environment::IW3SRDirectory / "features.json", std::ios::in | std::ios::out | std::ios::app);
		if (file.peek() != std::ifstream::traits_type::eof())
			Serialized = nlohmann::json::parse(file);
	}

	void Features::Serialize()
	{
		for (const auto& [_, entry] : Entries)
			entry->Serialize(Serialized[entry->ID]);

		std::ofstream file(Environment::IW3SRDirectory / "features.json");
		file << Serialized.dump(4);
		file.close();
	}
}

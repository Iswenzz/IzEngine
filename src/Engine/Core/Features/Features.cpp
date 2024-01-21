#include "Features.hpp"

namespace IW3SR::Engine
{
	void Features::Initialize()
	{
		Deserialize();
	}

	void Features::Release()
	{
		Serialize();
	}

	void Features::Remove(const std::string& id)
	{
		auto& entries = Get().Entries;
		if (auto it = entries.find(id); it != entries.end())
			entries.erase(it);
	}

	void Features::Deserialize()
	{
		std::ifstream file(Environment::IW3SRDirectory / "features.json");

		if (file.peek() != std::ifstream::traits_type::eof())
			Serialized = nlohmann::json::parse(file);

		for (const auto& [_, entry] : Entries)
		{
			entry->Initialize();
			entry->Deserialize(Serialized[entry->ID]);
		}
	}

	void Features::Serialize()
	{
		for (const auto& [_, entry] : Entries)
		{
			entry->Serialize(Serialized[entry->ID]);
			entry->Release();
		}
		std::ofstream file(Environment::IW3SRDirectory / "features.json");
		file << Serialized.dump(4);
		file.close();
	}
}

#include "Modules.hpp"
#include "Modules/Sys/Console.hpp"

namespace IW3SR
{
	Modules::Modules()
	{
		Load<Console>();
	}

	void Modules::Enable(std::string id)
	{
		auto entry = Entries.find(id);
		if (entry != Entries.end() && !entry->second->IsEnabled)
			entry->second->Initialize();
	}

	void Modules::Disable(std::string id)
	{
		auto entry = Entries.find(id);
		if (entry != Entries.end() && entry->second->IsEnabled)
			entry->second->Shutdown();
	}
}

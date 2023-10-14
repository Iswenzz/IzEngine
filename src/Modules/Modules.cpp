#include "Modules.hpp"
#include "Modules/Sys/Console.hpp"
#include "Modules/Player/ESP.hpp"
#include "Modules/Player/Velocity.hpp"
#include "Modules/Player/FPS.hpp"

namespace IW3SR
{
	void Modules::Initialize()
	{
		Load<Console>();
		Load<ESP>();
		Load<Velocity>();
		Load<FPS>();

		LoadDynamic();
	}

	void Modules::LoadDynamic()
	{

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

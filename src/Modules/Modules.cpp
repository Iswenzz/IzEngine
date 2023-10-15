#include "Modules.hpp"
#include "Modules/Sys/Console.hpp"
#include "Modules/Player/ESP.hpp"
#include "Modules/Player/FPS.hpp"
#include "Modules/Player/Velocity.hpp"

namespace IW3SR
{
	void Modules::Initialize()
	{
		Load<Console>();
		Load<ESP>();
		Load<FPS>();
		Load<Velocity>();

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

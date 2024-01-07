#include "Game/Plugin.hpp"
#include "Modules/Velocity.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->SetInfos("sr.player.velocity", "Velocity");

	Modules::Load<Velocity>();
}

PLUGIN void GUI()
{
	GUI::ModuleContext();
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.player.velocity");
}

#include "Game/Plugin.hpp"
#include "Modules/FPS.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->SetInfos("sr.player.fps", "FPS");

	Modules::Load<FPS>();
}

PLUGIN void GUI()
{
	GUI::ModuleContext();
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.player.fps");
}

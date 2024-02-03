#include "Game/Plugin.hpp"
#include "Modules/FPS.hpp"

PLUGIN void Initialize(Plugin* plugin)
{
	plugin->SetInfos("sr.player.fps", "FPS");

	Modules::Load<FPS>();
}

PLUGIN void Renderer()
{
	UI::InitializeContext();
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.player.fps");
}

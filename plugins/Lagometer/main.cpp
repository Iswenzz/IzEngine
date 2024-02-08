#include "Game/Plugin.hpp"
#include "Modules/Lagometer.hpp"

PLUGIN void Initialize(Plugin* plugin)
{
	plugin->SetInfos("sr.player.lagometer", "Lagometer");

	Modules::Load<Lagometer>();
}

PLUGIN void Renderer()
{
	UI::InitializeContext();
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.player.lagometer");
}

#include "Game/Plugin.hpp"
#include "Modules/Velocity.hpp"

PLUGIN void Initialize(Plugin* plugin)
{
	plugin->SetInfos("sr.player.velocity", "Velocity");

	Modules::Load<Velocity>();
}

PLUGIN void Renderer()
{
	GUI::ModuleContext();
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.player.velocity");
}

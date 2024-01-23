#include "Game/Plugin.hpp"
#include "Features/General.hpp"

PLUGIN void Initialize(Plugin* plugin)
{
	plugin->SetInfos("sr.feature.graphics", "Graphics");

	Features::Load<General>();
}

PLUGIN void Renderer()
{
	UI::ModuleContext();
}

PLUGIN void Shutdown()
{
	Features::Remove("sr.feature.graphics");
}

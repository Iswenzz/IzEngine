#include "Game/Plugin.hpp"
#include "Settings/General.hpp"

PLUGIN void Initialize(Plugin* plugin)
{
	plugin->SetInfos("sr.graphics", "Graphics");

	Settings::Load<General>();
}

PLUGIN void Renderer()
{
	UI::InitializeContext();
}

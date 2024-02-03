#include "Game/Plugin.hpp"
#include "Features/Graphics.hpp"

PLUGIN void Initialize(Plugin* plugin)
{
	plugin->SetInfos("sr.feature.graphics", "Graphics");

	Features::Load<Graphics>();
}

PLUGIN void Renderer()
{
	UI::InitializeContext();
}

PLUGIN void Shutdown()
{
	Features::Remove("sr.feature.graphics");
}

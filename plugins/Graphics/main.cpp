#include "Game/Plugin.hpp"
#include "Features/Graphics.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->SetInfos("sr.feature.graphics", "Graphics");

	Features::Load<Graphics>();
}

PLUGIN void GUI()
{
	GUI::ModuleContext();
}

PLUGIN void Shutdown()
{
	Features::Remove("sr.feature.graphics");
}

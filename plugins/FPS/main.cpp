#include "Sys/DLL.hpp"
#include "Modules/Modules.hpp"
#include "Modules/FPS.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->ID = "sr.player.fps";
	entry->Name = "FPS";

	Modules::Load<FPS>();
}

PLUGIN void GUI()
{
	ImGui::SetCurrentContext(GUI::Context);
	ImGui::SetAllocatorFunctions(GUI::Allocator, GUI::Free, GUI::Data);
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.player.fps");
}

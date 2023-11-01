#include "Sys/DLL.hpp"
#include "Modules/Modules.hpp"
#include "Modules/Velocity.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->ID = "sr.player.velocity";
	entry->Name = "Private";

	Modules::Load<Velocity>();
}

PLUGIN void GUI()
{
	ImGui::SetCurrentContext(GUI::Context);
	ImGui::SetAllocatorFunctions(GUI::Allocator, GUI::Free, GUI::Data);
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.player.velocity");
}

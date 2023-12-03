#include "Game/Modules.hpp"
#include "Modules/Velocity.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->SetInfos("sr.player.velocity", "Velocity");

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

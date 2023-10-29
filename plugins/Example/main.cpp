#include "Sys/DLL.hpp"
#include "Game/Render/GUI.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->ID = "sr.example";
	entry->Name = "Example";
}

PLUGIN void GUI()
{
	ImGui::SetCurrentContext(GUI::Context);
	ImGui::SetAllocatorFunctions(GUI::Allocator, GUI::Free, GUI::Data);
}

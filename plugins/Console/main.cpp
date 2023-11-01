#include "Sys/DLL.hpp"
#include "Modules/Modules.hpp"
#include "Modules/Console.hpp"

PLUGIN void Initialize(DLL* entry)
{
	entry->ID = "sr.sys.console";
	entry->Name = "Console";

	Modules::Load<Console>();
}

PLUGIN void GUI()
{
	ImGui::SetCurrentContext(GUI::Context);
	ImGui::SetAllocatorFunctions(GUI::Allocator, GUI::Free, GUI::Data);
}

PLUGIN void Shutdown()
{
	Modules::Remove("sr.sys.console");
}

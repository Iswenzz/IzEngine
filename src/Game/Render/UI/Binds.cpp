#include "Binds.hpp"

namespace IW3SR::UI
{
	Binds::Binds() : Window("Binds") { }
	
	void Binds::Frame() 
	{
		if (!Open) return;

		Begin();
		if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
			ImGui::Keybind("Menu", &GUI::OpenKey.Key);
		End();
	}
}

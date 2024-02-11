#include "Binds.hpp"

namespace IW3SR::Game::UC
{
	Binds::Binds() : Window("Binds") { }

	void Binds::Render()
	{
		if (!Open) return;

		Begin();
		if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
			ImGui::Keybind("Menu", &GUI::Get().OpenKey.Key);
		End();
	}
}

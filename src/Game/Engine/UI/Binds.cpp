#include "Binds.hpp"

namespace IW3SR::Game::UC
{
	Binds::Binds() : Window("Binds") { }

	void Binds::Render()
	{
		if (!Open) return;
		const ImVec2 size = { ImGui::CalcItemWidth(), 0 };

		Begin();
		if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen))
			ImGui::Keybind("Menu", &UI::Get().OpenKey.Key, size);
		End();
	}
}

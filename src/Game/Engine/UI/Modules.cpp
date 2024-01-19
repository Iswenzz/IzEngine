#include "Modules.hpp"
#include "Game/Game.hpp"

#include <set>

namespace IW3SR::UI
{
	Modules::Modules() : Window("Modules")
	{
		SetRect(470, 20, 150, 185);
	}

	void Modules::Render()
	{
		if (!Open) return;

		Begin();
		const float frameWidth = ImGui::GetWindowContentRegionMax().x - 30;
		std::set<std::string> groups;

		for (const auto& [_, current] : GetRenderer()->Modules->Entries)
		{
			if (std::ranges::find(groups, current->Group) != groups.end())
				continue;

			groups.insert(current->Group);
			if (!ImGui::CollapsingHeader(current->Group.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				continue;

			for (const auto& [_, entry] : GetRenderer()->Modules->Entries)
			{
				if (current->Group != entry->Group)
					continue;

				// Enable/Disable module
				if (ImGui::Toggle(entry->ID + "toggle", 20, &entry->IsEnabled))
					entry->IsEnabled ? entry->Initialize() : entry->Release();
				ImGui::SameLine();
				ImGui::Text(entry->Name.c_str());
				ImGui::SameLine(frameWidth);

				// Draw module menu
				ImGui::Button(ICON_FA_GEAR, entry->ID + "menu", &entry->Menu.Open);
				if (entry->Menu.Open)
				{
					entry->Menu.Begin();
					entry->OnMenu();
					entry->Menu.End();
				}
			}
		}
		End();
	}
}

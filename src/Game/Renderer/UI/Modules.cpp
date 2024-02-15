#include "Modules.hpp"
#include <set>

namespace IW3SR::Game::UC
{
	Modules::Modules() : Window("Modules")
	{
		SetRect(-170, 20, 150, 185);
		SetRectAlignment(HORIZONTAL_RIGHT, VERTICAL_TOP);
	}

	void Modules::Render()
	{
		if (!Open) return;

		Begin();
		auto& modules = Game::Modules::Get();
		const float frameWidth = ImGui::GetWindowContentRegionMax().x - 30;
		std::set<std::string> groups;

		for (const auto& [_, current] : modules.Entries)
		{
			if (std::ranges::find(groups, current->Group) != groups.end())
				continue;

			groups.insert(current->Group);
			if (!ImGui::CollapsingHeader(current->Group.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				continue;

			for (const auto& [_, entry] : modules.Entries)
			{
				if (current->Group != entry->Group)
					continue;

				// Enable/Disable module
				if (ImGui::Toggle(entry->ID + "toggle", &entry->IsEnabled))
					entry->IsEnabled ? modules.Enable(entry->ID) : modules.Disable(entry->ID);
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

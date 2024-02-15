#include "Settings.hpp"
#include <set>

namespace IW3SR::Game::UC
{
	Settings::Settings() : Window("Settings")
	{
		SetRect(-170, 20, 150, 100);
		SetRectAlignment(HORIZONTAL_RIGHT, VERTICAL_TOP);
	}

	void Settings::Render()
	{
		if (!Open) return;

		Begin();
		auto& features = Game::Features::Get();
		const float frameWidth = ImGui::GetWindowContentRegionMax().x - 30;
		std::set<std::string> groups;

		for (const auto& [_, current] : features.Entries)
		{
			if (std::ranges::find(groups, current->Group) != groups.end())
				continue;

			groups.insert(current->Group);
			if (!ImGui::CollapsingHeader(current->Group.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				continue;

			for (const auto& [_, entry] : features.Entries)
			{
				if (current->Group != entry->Group)
					continue;

				ImGui::Text(entry->Name.c_str());
				ImGui::SameLine(frameWidth);

				// Draw feature menu
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

#include "Modules.hpp"
#include "Modules/Sys/Console.hpp"
#include "Modules/Player/FPS.hpp"
#include "Modules/Player/Velocity.hpp"

namespace IW3SR
{
	Modules::Modules()
	{
		Menu = Window("Modules");
		Menu.SetRect(470, 20, 150, 100);

		Deserialize();
	}

	Modules::~Modules()
	{
		Serialize();
	}

	void Modules::Initialize()
	{
		Load<Console>();
		Load<FPS>();
		Load<Velocity>();
		LoadDynamic();
	}

	void Modules::LoadDynamic()
	{

	}

	void Modules::Enable(const std::string& id)
	{
		auto entry = Entries.find(id);
		if (entry != Entries.end() && !entry->second->IsEnabled)
		{
			entry->second->IsEnabled = true;
			entry->second->Initialize();
		}
	}

	void Modules::Disable(const std::string& id)
	{
		auto entry = Entries.find(id);
		if (entry != Entries.end() && entry->second->IsEnabled)
		{
			entry->second->IsEnabled = false;
			entry->second->Shutdown();
		}
	}

	void Modules::Deserialize()
	{
		Environment::GetSettings("modules", *this);

		std::fstream file(Environment::IW3SRDirectory / "modules.json", std::ios::in | std::ios::out | std::ios::app);
		if (file.peek() != std::ifstream::traits_type::eof())
			Serialized = nlohmann::json::parse(file);
	}

	void Modules::Serialize()
	{
		Environment::SetSettings("modules", *this);

		for (const auto& [_, entry] : Entries)
			entry->Serialize(Serialized[entry->ID]);

		std::ofstream file(Environment::IW3SRDirectory / "modules.json");
		file << Serialized.dump(4);
		file.close();
	}

	void Modules::Frame()
	{
		if (!Menu.Open) return;

		Menu.Begin();
		const float frameWidth = ImGui::GetWindowContentRegionMax().x - 30;

		for (const auto& [_, entry] : Entries)
		{
			const char* name = entry->Name.c_str();

			// Enable/Disable module
			if (ImGui::ToggleButton(entry->ID + "toggle", 20, &entry->IsEnabled))
				entry->IsEnabled ? entry->Initialize() : entry->Shutdown();
			ImGui::SameLine();
			ImGui::Text(name);
			ImGui::SameLine(frameWidth);

			// Draw module menu
			ImGui::ButtonId(ICON_FA_GEAR, entry->ID + "menu", &entry->Menu.Open);
			if (entry->Menu.Open)
			{
				entry->Menu.Begin();
				entry->OnMenu();
				entry->Menu.End();
			}
		}
		Menu.End();
	}
}

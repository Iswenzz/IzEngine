#include "Modules.hpp"

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
		if (!std::filesystem::is_directory(Environment::PluginsDirectory))
			return;

		for (const auto& entry : std::filesystem::directory_iterator(Environment::PluginsDirectory))
		{
			if (entry.path().extension() == ".dll")
				DLLs.insert({ entry.path().filename().string(), std::make_unique<DLL>(entry.path().string()) });
		}
	}

	void Modules::Reload()
	{
		DLLs.clear();

		if (std::filesystem::exists(CMAKE_BINARY_DIR))
		{
			constexpr auto command = R"(cd "{}" && cmake --build . --config Debug --target Install)";
			system(std::format(command, CMAKE_BINARY_DIR).c_str());
		}
		Initialize();
	}

	void Modules::SetRenderer()
	{
		for (const auto& [_, dll] : DLLs)
			dll->SetRenderer();
	}

	void Modules::Enable(const std::string& id)
	{
		auto& entry = Entries[id];
		entry->IsEnabled = true;
		entry->Initialize();
	}

	void Modules::Disable(const std::string& id)
	{
		auto& entry = Entries[id];
		entry->IsEnabled = false;
		entry->Shutdown();
	}

	void Modules::Remove(const std::string& id)
	{
		auto it = Entries.find(id);
		if (it != Entries.end())
			Entries.erase(it);
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

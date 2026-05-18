#pragma once
#include "Engine/Core/Interfaces/ISerializable.hpp"

namespace IzEngine
{
	enum class Directory
	{
		Base,
		App,
		Bin,
		Configs,
		Plugins,
		Resources,
		Reports,
	};

	class API Environment
	{
	public:
		static inline bool Initialized = false;

		static void Binary();
		static void Local();

		static void Load(nlohmann::json& json, const std::string& filename);
		static void Save(const nlohmann::json& json, const std::string& filename);
		static const std::filesystem::path& Path(Directory directory);

	private:
		static inline std::unordered_map<Directory, std::filesystem::path> Directories;

		static void Initialize();
	};
}

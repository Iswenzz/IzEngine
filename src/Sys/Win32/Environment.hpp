#pragma once
#include "Sys/Win32.hpp"

#include <filesystem>
#include <cstdlib>

namespace IW3SR
{
	/// <summary>
	/// System environment.
	/// </summary>
	class API Environment
	{
	public:
		static inline nlohmann::json Settings;
		static inline std::vector<std::string> Modules;

		static inline std::filesystem::path BaseDirectory;
		static inline std::filesystem::path IW3SRDirectory;
		static inline std::filesystem::path PluginsDirectory;
		static inline std::filesystem::path ResourcesDirectory;
		static inline std::filesystem::path FontsDirectory;
		static inline std::filesystem::path ImagesDirectory;

		/// <summary>
		/// Initialize the environment.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Load settings.
		/// </summary>
		static void Load();

		/// <summary>
		/// Save environment.
		/// </summary>
		static void Save();

		/// <summary>
		/// Deserialize the settings.
		/// </summary>
		/// <typeparam name="T">The class implementing serialization.</typeparam>
		/// <param name="id">The settings id.</param>
		/// <param name="instance">The class instance.</param>
		template <typename T>
		static void Deserialize(const std::string& id, T& instance)
		{
			if (Settings.contains(id))
				nlohmann::from_json(Settings[id], instance);
		}

		/// <summary>
		/// Serialize the settings.
		/// </summary>
		/// <typeparam name="T">The class implementing serialization.</typeparam>
		/// <param name="id">The settings id.</param>
		/// <param name="instance">The class instance.</param>
		template <typename T>
		static void Serialize(const std::string& id, const T& instance)
		{
			nlohmann::to_json(Settings[id], instance);
		}

	private:
		/// <summary>
		/// Build the environment.
		/// </summary>
		static void Build();

		/// <summary>
		/// Build the modules list.
		/// </summary>
		static void BuildModulesList();
	};
}

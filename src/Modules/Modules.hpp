#pragma once
#include "Game/Game.hpp"
#include "Sys/DLL.hpp"
#include "Sys/Log.hpp"
#include "Module.hpp"

#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <memory>

#define ModuleCallback(method, ...) \
	SR->Modules->Callback([&](const std::unique_ptr<Module>& entry) { entry->method(__VA_ARGS__); });

namespace IW3SR
{
	/// <summary>
	/// Game modules.
	/// </summary>
	class API Modules
	{
	public:
		static inline std::map<std::string, std::unique_ptr<Module>> Entries;
		static inline std::map<std::string, std::unique_ptr<DLL>> DLLs;
		static inline nlohmann::json Serialized;
		static inline Window Menu;
		static inline bool IsReloading = false;

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		Modules();
		~Modules();

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Reload the modules.
		/// </summary>
		void Reload();

		/// <summary>
		/// Compile the plugins.
		/// </summary>
		void Compile();

		/// <summary>
		/// Load a module.
		/// </summary>
		/// <typeparam name="M">The module type.</typeparam>
		template <class M = Module>
		static void Load(bool initialize = true)
		{
			std::unique_ptr<M> entry = std::make_unique<M>();
			bool isSerialized = Serialized.contains(entry->ID);

			entry->IsEnabled = initialize;
			if (isSerialized)
			{
				try { entry->Deserialize(Serialized[entry->ID]); }
				catch (...) { }
			}
			if (entry->IsEnabled)
				entry->Initialize();

			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Set the dynamic modules renderer.
		/// </summary>
		void SetRenderer();

		/// <summary>
		/// Enable a module.
		/// </summary>
		/// <param name="id">The module id.</param>
		void Enable(const std::string& id);

		/// <summary>
		/// Disable a module.
		/// </summary>
		/// <param name="id">The module id.</param>
		void Disable(const std::string& id);

		/// <summary>
		/// Remove a module.
		/// </summary>
		/// <param name="id">The module id.</param>
		static void Remove(const std::string& id);

		/// <summary>
		/// Deserialize the modules.
		/// </summary>
		void Deserialize();

		/// <summary>
		/// Serialize the modules.
		/// </summary>
		void Serialize();

		/// <summary>
		/// Dispatch callback.
		/// </summary>
		/// <typeparam name="Func">The callback type.</typeparam>
		/// <param name="callback">The function callback.</param>
		template <typename Func>
		void Callback(Func callback)
		{
			for (const auto& [_, entry] : Entries)
			{
				if (entry->IsEnabled)
					callback(entry);
			}
		}

		/// <summary>
		/// Render the modules menu.
		/// </summary>
		void Frame();

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Modules, Menu);
	};
}

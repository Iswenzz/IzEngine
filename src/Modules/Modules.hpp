#pragma once
#include "Module.hpp"

#include <string>
#include <map>
#include <memory>

#define ModuleCallback(method, ...) \
	SR->Modules->Callback([&](const std::unique_ptr<Module>& entry) { entry->method(__VA_ARGS__); });

namespace IW3SR
{
	/// <summary>
	/// Game modules.
	/// </summary>
	class Modules
	{
	public:
		std::map<std::string, std::unique_ptr<Module>> Entries;

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		Modules() = default;
		~Modules() = default;

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Load a module.
		/// </summary>
		/// <typeparam name="M">The module type.</typeparam>
		template <class M = Module>
		void Load(bool initialize = true)
		{
			std::unique_ptr<M> entry = std::make_unique<M>();
			if (initialize)
				entry->Initialize();
			entry->IsEnabled = initialize;

			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Load dynamic modules.
		/// </summary>
		void LoadDynamic();

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
	};
}

#pragma once
#include "Module.hpp"

#include <string>
#include <map>
#include <memory>

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
		Modules();
		~Modules() = default;

		/// <summary>
		/// Load a module.
		/// </summary>
		/// <typeparam name="M">The module type.</typeparam>
		template <class M = Module>
		void Load()
		{
			std::unique_ptr<M> entry = std::make_unique<M>();
			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Enable a module.
		/// </summary>
		/// <param name="id">The module id.</param>
		void Enable(std::string id);

		/// <summary>
		/// Disable a module.
		/// </summary>
		/// <param name="id">The module id.</param>
		void Disable(std::string id);
	};
}

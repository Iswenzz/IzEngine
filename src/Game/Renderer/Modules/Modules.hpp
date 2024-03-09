#pragma once
#include "Module.hpp"

#include <memory>
#include <map>
#include <string>

namespace IW3SR::Game
{
	/// <summary>
	/// Modules class.
	/// </summary>
	class API Modules
	{
		CLASS_SINGLETON(Modules)
	public:
		std::map<std::string, std::shared_ptr<Module>> Entries;
		nlohmann::json Serialized;

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Release the modules.
		/// </summary>
		void Release();

		/// <summary>
		/// Add a module.
		/// </summary>
		/// <typeparam name="M">The module type.</typeparam>
		/// <param name="enabled">Is enabled by default.</param>
		template <class M = Module>
		void Add(bool enabled = true)
		{
			auto entry = std::make_shared<M>();
			bool isSerialized = Serialized.contains(entry->ID);

			entry->IsEnabled = enabled;
			if (isSerialized)
				entry->Deserialize(Serialized[entry->ID]);

			Entries.insert({ entry->ID, std::move(entry) });
		}

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
		void Remove(const std::string& id);

		/// <summary>
		/// Load the modules.
		/// </summary>
		void Deserialize();

		/// <summary>
		/// Serialize the modules.
		/// </summary>
		void Serialize();

		/// <summary>
		/// Dipatch event.
		/// </summary>
		/// <param name="event">The event.</param>
		void Dispatch(Event& event);

		/// <summary>
		/// Load a module.
		/// </summary>
		/// <typeparam name="M">The module type.</typeparam>
		/// <param name="enabled">Is enabled by default.</param>
		template <class M = Module>
		static void Load(bool enabled = true)
		{
			Get().Add<M>(enabled);
		}

	protected:
		/// <summary>
		/// Initialize the modules.
		/// </summary>
		Modules() = default;
		virtual ~Modules() = default;
	};
}

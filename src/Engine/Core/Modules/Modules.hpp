#pragma once
#include "Module.hpp"

namespace IW3SR
{
	/// <summary>
	/// Game modules.
	/// </summary>
	class API Modules : public ISerializable
	{
	public:
		static inline std::map<std::string, std::unique_ptr<Module>> Entries;
		static inline nlohmann::json Serialized;

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		Modules() = default;
		virtual ~Modules() = default;

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Load a module.
		/// </summary>
		/// <typeparam name="M">The module type.</typeparam>
		template <class M = Module>
		static void Load(bool enabled = true)
		{
			auto entry = std::make_unique<M>();
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
		virtual void Enable(const std::string& id);

		/// <summary>
		/// Disable a module.
		/// </summary>
		/// <param name="id">The module id.</param>
		virtual void Disable(const std::string& id);

		/// <summary>
		/// Remove a module.
		/// </summary>
		/// <param name="id">The module id.</param>
		static void Remove(const std::string& id);

		/// <summary>
		/// Dispatch callback.
		/// </summary>
		/// <typeparam name="Func">The callback type.</typeparam>
		/// <param name="callback">The function callback.</param>
		template <typename Func>
		static void Callback(Func callback)
		{
			for (const auto& [_, entry] : Entries)
			{
				if (entry->IsEnabled)
					callback(entry);
			}
		}

		/// <summary>
		/// Load the modules.
		/// </summary>
		virtual void Deserialize() override;

		/// <summary>
		/// Serialize the modules.
		/// </summary>
		virtual void Serialize() override;

		/// <summary>
		/// Shutdown the modules.
		/// </summary>
		virtual void Shutdown();
	};
}

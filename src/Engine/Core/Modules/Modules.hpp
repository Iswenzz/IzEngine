#pragma once
#include "Module.hpp"

#include <memory>
#include <map>
#include <string>

#define ModulesCallback(method, ...) \
	Modules::Get().Callback([&](const auto& entry) { entry->method(__VA_ARGS__); });

namespace IW3SR::Engine
{
	/// <summary>
	/// Modules class.
	/// </summary>
	class API Modules : public IInitializable
	{
		CLASS_SINGLETON(Modules)
	public:
		std::map<std::string, std::shared_ptr<Module>> Entries;
		nlohmann::json Serialized;

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Release the modules.
		/// </summary>
		virtual void Release();

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
		/// Load a module.
		/// </summary>
		/// <typeparam name="M">The module type.</typeparam>
		/// <param name="enabled">Is enabled by default.</param>
		template <class M = Module>
		static void Load(bool enabled = true)
		{
			Get().Add<M>(enabled);
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
		void Callback(Func callback)
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
		virtual void Deserialize();

		/// <summary>
		/// Serialize the modules.
		/// </summary>
		virtual void Serialize();

	protected:
		/// <summary>
		/// Initialize the modules.
		/// </summary>
		Modules() = default;
		virtual ~Modules() = default;
	};
}

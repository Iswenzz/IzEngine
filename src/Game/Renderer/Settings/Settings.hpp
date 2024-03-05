#pragma once
#include "Setting.hpp"

#include <memory>
#include <map>
#include <string>

#define SettingsCallback(method, ...) \
	Settings::Get().Callback([&](const auto& entry) { entry->method(__VA_ARGS__); });

namespace IW3SR::Game
{
	/// <summary>
	/// Engine settings.
	/// </summary>
	class API Settings : public IInitializable
	{
		CLASS_SINGLETON(Settings)
	public:
		std::map<std::string, std::shared_ptr<Setting>> Entries;
		nlohmann::json Serialized;

		/// <summary>
		/// Initialize the settings.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Release the settings.
		/// </summary>
		virtual void Release();

		/// <summary>
		/// Add a setting.
		/// </summary>
		/// <typeparam name="F">The setting type.</typeparam>
		template <class F = Setting>
		void Add()
		{
			auto entry = std::make_shared<F>();
			bool isSerialized = Serialized.contains(entry->ID);

			if (isSerialized)
				entry->Deserialize(Serialized[entry->ID]);

			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Load a setting.
		/// </summary>
		/// <typeparam name="F">The setting type.</typeparam>
		template <class F = Setting>
		static void Load()
		{
			Get().Add<F>();
		}

		/// <summary>
		/// Remove a setting.
		/// </summary>
		/// <param name="id">The setting id.</param>
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
				callback(entry);
		}

		/// <summary>
		/// Load the settings.
		/// </summary>
		virtual void Deserialize();

		/// <summary>
		/// Serialize the settings.
		/// </summary>
		virtual void Serialize();

	protected:
		/// <summary>
		/// Initialize the settings.
		/// </summary>
		Settings() = default;
		virtual ~Settings() = default;
	};
}

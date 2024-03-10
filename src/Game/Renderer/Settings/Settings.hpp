#pragma once
#include "Setting.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Engine settings.
	/// </summary>
	class API Settings
	{
		CLASS_SINGLETON(Settings)
	public:
		std::map<std::string, Ref<Setting>> Entries;
		nlohmann::json Serialized;

		/// <summary>
		/// Initialize the settings.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Release the settings.
		/// </summary>
		void Release();

		/// <summary>
		/// Add a setting.
		/// </summary>
		/// <typeparam name="F">The setting type.</typeparam>
		template <class F = Setting>
		void Add()
		{
			auto entry = CreateRef<F>();
			bool isSerialized = Serialized.contains(entry->ID);

			if (isSerialized)
				entry->Deserialize(Serialized[entry->ID]);

			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Remove a setting.
		/// </summary>
		/// <param name="id">The setting id.</param>
		void Remove(const std::string& id);

		/// <summary>
		/// Load the settings.
		/// </summary>
		void Deserialize();

		/// <summary>
		/// Serialize the settings.
		/// </summary>
		void Serialize();

		/// <summary>
		/// Dispatch event.
		/// </summary>
		/// <param name="event">The event.</param>
		void Dispatch(Event& event);

		/// <summary>
		/// Load a setting.
		/// </summary>
		/// <typeparam name="F">The setting type.</typeparam>
		template <class F = Setting>
		static void Load()
		{
			Get().Add<F>();
		}

	protected:
		/// <summary>
		/// Initialize the settings.
		/// </summary>
		Settings() = default;
		virtual ~Settings() = default;
	};
}

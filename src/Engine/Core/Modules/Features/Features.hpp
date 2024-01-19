#pragma once
#include "Feature.hpp"
#include "Engine/Core/Modules/Modules.hpp"

namespace IW3SR
{
	/// <summary>
	/// Game features.
	/// </summary>
	class API Features : public Modules
	{
	public:
		static inline std::map<std::string, std::unique_ptr<Feature>> Entries;
		static inline nlohmann::json Serialized;

		/// <summary>
		/// Initialize the features.
		/// </summary>
		Features() = default;
		virtual ~Features() = default;

		/// <summary>
		/// Load a feature.
		/// </summary>
		/// <typeparam name="F">The feature type.</typeparam>
		template <class F = Feature>
		static void Load()
		{
			auto entry = std::make_unique<F>();
			bool isSerialized = Serialized.contains(entry->ID);

			entry->IsEnabled = true;
			if (isSerialized)
				entry->Deserialize(Serialized[entry->ID]);

			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Remove a feature.
		/// </summary>
		/// <param name="id">The feature id.</param>
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
		/// Load the features.
		/// </summary>
		void Deserialize() override;

		/// <summary>
		/// Serialize the features.
		/// </summary>
		void Serialize() override;
	};
}

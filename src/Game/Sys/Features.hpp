#pragma once
#include "Feature.hpp"

namespace IW3SR
{
	/// <summary>
	/// Game features.
	/// </summary>
	class API Features
	{
	public:
		static inline std::map<std::string, std::unique_ptr<Feature>> Entries;
		static inline nlohmann::json Serialized;

		/// <summary>
		/// Initialize the features.
		/// </summary>
		Features();
		~Features();
		
		/// <summary>
		/// Load a feature.
		/// </summary>
		/// <typeparam name="F">The feature type.</typeparam>
		template <class F = Feature>
		static void Load()
		{
			std::unique_ptr<F> entry = std::make_unique<F>();
			bool isSerialized = Serialized.contains(entry->ID);

			if (isSerialized)
			{
				try { entry->Deserialize(Serialized[entry->ID]); }
				catch (...) {}
			}
			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Remove a feature.
		/// </summary>
		/// <param name="id">The feature id.</param>
		static void Remove(const std::string& id);

		/// <summary>
		/// Deserialize the features.
		/// </summary>
		void Deserialize();

		/// <summary>
		/// Serialize the features.
		/// </summary>
		void Serialize();
	};
}

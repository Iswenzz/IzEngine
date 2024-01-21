#pragma once
#include "Feature.hpp"

#include <memory>
#include <map>
#include <string>

#define FeaturesCallback(method, ...) \
	Features::Get().Callback([&](const auto& entry) { entry->method(__VA_ARGS__); });

namespace IW3SR::Engine
{
	/// <summary>
	/// Features class.
	/// </summary>
	class API Features : public IInitializable
	{
		CLASS_SINGLETON(Features)
	public:
		std::map<std::string, std::shared_ptr<Feature>> Entries;
		nlohmann::json Serialized;

		/// <summary>
		/// Initialize the features.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Release the features.
		/// </summary>
		virtual void Release();

		/// <summary>
		/// Add a feature.
		/// </summary>
		/// <typeparam name="F">The feature type.</typeparam>
		template <class F = Feature>
		void Add()
		{
			auto entry = std::make_shared<F>();
			bool isSerialized = Serialized.contains(entry->ID);

			if (isSerialized)
				entry->Deserialize(Serialized[entry->ID]);

			Entries.insert({ entry->ID, std::move(entry) });
		}

		/// <summary>
		/// Load a feature.
		/// </summary>
		/// <typeparam name="F">The feature type.</typeparam>
		template <class F = Feature>
		static void Load()
		{
			Get().Add<F>();
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
		void Callback(Func callback)
		{
			for (const auto& [_, entry] : Entries)
				callback(entry);
		}

		/// <summary>
		/// Load the features.
		/// </summary>
		virtual void Deserialize();

		/// <summary>
		/// Serialize the features.
		/// </summary>
		virtual void Serialize();

	protected:
		/// <summary>
		/// Initialize the features.
		/// </summary>
		Features() = default;
		virtual ~Features() = default;
	};
}

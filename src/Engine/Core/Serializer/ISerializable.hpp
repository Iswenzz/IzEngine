#pragma once
#include "Serializer.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Serializable class.
	/// </summary>
	class API ISerializable
	{
	public:
		/// <summary>
		/// Serialize to json.
		/// </summary>
		/// <param name="json">The json object.</param>
		virtual void Serialize(nlohmann::json& json) { };

		/// <summary>
		/// Deserialize to json.
		/// </summary>
		/// <param name="json">The json object.</param>
		virtual void Deserialize(const nlohmann::json& json) { };

		/// <summary>
		/// Assign value from json.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="json">The json object.</param>
		/// <param name="value">The value.</param>
		template <class T>
		requires IsSerializable<T>
		static inline void From(const nlohmann::json& json, T& value)
		{
			value.Deserialize(json);
		}

		/// <summary>
		/// Assign value from json.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="json">The json object.</param>
		/// <param name="value">The value.</param>
		template <typename T>
		static inline void From(const nlohmann::json& json, T& value)
		{
			json.get_to(value);
		}

		/// <summary>
		/// Assign value to json.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="json">The json object.</param>
		/// <param name="value">The value.</param>
		template <class T>
		requires IsSerializable<T>
		static inline void To(nlohmann::json& json, const T& value)
		{
			value.Serialize(json);
		}

		/// <summary>
		/// Assign value to json.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="json">The json object.</param>
		/// <param name="value">The value.</param>
		template <typename T>
		static inline void To(nlohmann::json& json, const T& value)
		{
			json = value;
		}
	};

	template <class T>
	concept IsSerializable = std::is_base_of_v<ISerializable, T>;
}

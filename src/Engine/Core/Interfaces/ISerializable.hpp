#pragma once
#include "Engine/Base.hpp"

#include <nlohmann/json.hpp>

namespace IzEngine
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
		virtual void Serialize(nlohmann::json& json) {};

		/// <summary>
		/// Deserialize to json.
		/// </summary>
		/// <param name="json">The json object.</param>
		virtual void Deserialize(const nlohmann::json& json) {};

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

	protected:
		/// <summary>
		/// Create serializable.
		/// </summary>
		ISerializable() = default;

		/// <summary>
		/// Release serializable.
		/// </summary>
		virtual ~ISerializable() = default;
	};

	template <class T>
	concept IsSerializable = std::is_base_of_v<ISerializable, T>;
}

#ifdef NLOHMANN_JSON_FROM
	#undef NLOHMANN_JSON_FROM
	#define NLOHMANN_JSON_FROM(v1)         \
		if (nlohmann_json_j.contains(#v1)) \
			ISerializable::From(nlohmann_json_j[#v1], nlohmann_json_t.v1);
#endif

#ifdef NLOHMANN_JSON_TO
	#undef NLOHMANN_JSON_TO
	#define NLOHMANN_JSON_TO(v1) ISerializable::To(nlohmann_json_j[#v1], nlohmann_json_t.v1);
#endif

#define SERIALIZE(Type, ...) NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, __VA_ARGS__)
#define SERIALIZE_NON_INTRUSIVE(Type, ...) NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Type, __VA_ARGS__)

#define SERIALIZE_DERIVED(Type, DerivedType, ...)                                             \
	friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t)         \
	{                                                                                         \
		nlohmann::to_json(nlohmann_json_j, static_cast<const DerivedType&>(nlohmann_json_t)); \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__))              \
	}                                                                                         \
	friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t)       \
	{                                                                                         \
		nlohmann::from_json(nlohmann_json_j, static_cast<DerivedType&>(nlohmann_json_t));     \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__))            \
	}

#define SERIALIZE_DERIVED_EMPTY(Type, DerivedType)                                            \
	friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t)         \
	{                                                                                         \
		nlohmann::to_json(nlohmann_json_j, static_cast<const DerivedType&>(nlohmann_json_t)); \
	}                                                                                         \
	friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t)       \
	{                                                                                         \
		nlohmann::from_json(nlohmann_json_j, static_cast<DerivedType&>(nlohmann_json_t));     \
	}

#define SERIALIZE_POLY_BASE(Type, ...)                                             \
public:                                                                            \
	virtual void Serialize(nlohmann::json& nlohmann_json_j) const                  \
	{                                                                              \
		const Type& nlohmann_json_t = *this;                                       \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__))   \
	}                                                                              \
	virtual void Deserialize(const nlohmann::json& nlohmann_json_j)                \
	{                                                                              \
		Type& nlohmann_json_t = *this;                                             \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) \
	}

#define SERIALIZE_POLY(Type, DerivedType, ...)                                     \
public:                                                                            \
	virtual void Serialize(nlohmann::json& nlohmann_json_j) const override         \
	{                                                                              \
		const Type& nlohmann_json_t = *this;                                       \
		DerivedType::Serialize(nlohmann_json_j);                                   \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__))   \
	}                                                                              \
	virtual void Deserialize(const nlohmann::json& nlohmann_json_j) override       \
	{                                                                              \
		Type& nlohmann_json_t = *this;                                             \
		DerivedType::Deserialize(nlohmann_json_j);                                 \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) \
	}

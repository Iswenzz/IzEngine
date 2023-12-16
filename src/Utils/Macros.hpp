#pragma once

#ifndef CDECL
#define CDECL __cdecl
#endif

#ifndef STDCALL
#define STDCALL __stdcall
#endif

#ifndef FASTCALL
#define FASTCALL __fastcall
#endif

#define C_EXTERN extern "C"
#define CPP_EXTERN extern "C++"
#define ASM C_EXTERN

#ifndef EXPORT
#define EXPORT __declspec(dllexport)
#endif

#ifndef IMPORT
#define IMPORT __declspec(dllimport)
#endif

#ifndef API
#define API EXPORT
#endif

#ifndef PLUGIN
#define PLUGIN C_EXTERN EXPORT
#endif

#ifndef ENTRY
#define ENTRY C_EXTERN EXPORT
#endif

#ifndef CMAKE_BINARY_DIR
#define CMAKE_BINARY_DIR ""
#endif

#define NLOHMANN_SERIALIZE(Type, ...) \
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__)

#define NLOHMANN_SERIALIZE_NON_INTRUSIVE(Type, ...) \
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__)

#define NLOHMANN_SERIALIZE_NON_INTRUSIVE_RAW(Type, ...) \
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Type, __VA_ARGS__)

#define NLOHMANN_SERIALIZE_DERIVED(Type, BaseType, ...) \
    friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) \
	{ \
		nlohmann::to_json(nlohmann_json_j, static_cast<const BaseType &>(nlohmann_json_t)); \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)); \
	} \
    friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) \
	{ \
		Type nlohmann_json_default_obj; \
		nlohmann::from_json(nlohmann_json_j, static_cast<BaseType&>(nlohmann_json_t)); \
		NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM_WITH_DEFAULT, __VA_ARGS__)); \
	}

#define NLOHMANN_SERIALIZE_DERIVED_EMPTY(Type, BaseType) \
    friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) \
	{ \
		nlohmann::to_json(nlohmann_json_j, static_cast<const BaseType &>(nlohmann_json_t)); \
	} \
    friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) \
	{ \
		nlohmann::from_json(nlohmann_json_j, static_cast<BaseType&>(nlohmann_json_t)); \
	}

#define NLOHMANN_SERIALIZE_POLY(Type, BaseType, ...) \
	virtual void Serialize(nlohmann::json& nlohmann_json_j) override \
	{ \
		BaseType::Serialize(nlohmann_json_j); \
		to_json(nlohmann_json_j, *this); \
	} \
	virtual void Deserialize(nlohmann::json& nlohmann_json_j) override \
	{ \
		BaseType::Deserialize(nlohmann_json_j); \
		from_json(nlohmann_json_j, *this); \
	} \
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__);

#define NLOHMANN_SERIALIZE_POLY_BASE(Type, ...) \
	virtual void Serialize(nlohmann::json& nlohmann_json_j) \
	{ \
		to_json(nlohmann_json_j, *this); \
	} \
	virtual void Deserialize(nlohmann::json& nlohmann_json_j) \
	{ \
		from_json(nlohmann_json_j, *this); \
	} \
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__);

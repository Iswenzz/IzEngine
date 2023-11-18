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

#ifndef DEBUG
	#ifndef _DEBUG
		#define _DEBUG 0
	#endif
	#define DEBUG _DEBUG
#endif

#ifndef CMAKE_BINARY_DIR
#define CMAKE_BINARY_DIR ""
#endif

#define NLOHMANN_DEFINE_POLY(Type, ChildClass, ...) \
	virtual void Serialize(nlohmann::json& json) override \
	{ \
		ChildClass::Serialize(json); \
		to_json(json, *this); \
	} \
	virtual void Deserialize(nlohmann::json& json) override \
	{ \
		ChildClass::Deserialize(json); \
		from_json(json, *this); \
	} \
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, __VA_ARGS__);

#define NLOHMANN_DEFINE_POLY_BASE(Type, ...) \
	virtual void Serialize(nlohmann::json& json) \
	{ \
		to_json(json, *this); \
	} \
	virtual void Deserialize(nlohmann::json& json) \
	{ \
		from_json(json, *this); \
	} \
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Type, __VA_ARGS__);

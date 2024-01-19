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

#define CLASS_DISALLOW_COPY_AND_ASSIGN(type) \
	type(const type&) = delete; \
	void operator=(const type&) = delete

#define CLASS_SINGLETON(type) \
public: \
	static inline type* GetInstance() \
	{ \
		if (!Instance) \
			Instance = std::make_unique<type>(); \
		return Instance.get(); \
	} \
private: \
	CLASS_DISALLOW_COPY_AND_ASSIGN(type); \
	static inline std::unique_ptr<type> Instance;

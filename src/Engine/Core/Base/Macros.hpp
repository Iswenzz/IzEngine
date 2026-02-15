#pragma once

#if defined(_MSC_VER)
	#include "Core/Compilers/MSVC.hpp"
#elif defined(__clang__)
	#include "Core/Compilers/Clang.hpp"
#elif defined(__GNUC__)
	#include "Core/Compilers/GCC.hpp"
#else
	#error Unsupported compiler
#endif

#if !defined(PLATFORM_32) && !defined(PLATFORM_64)
	#error Unsupported address size
#elif !defined(PLATFORM_ARCH_X86) && !defined(PLATFORM_ARCH_ARM)
	#error Unsupported architecture
#elif !defined(PLATFORM_WINDOWS) && !defined(PLATFORM_LINUX)
	#error Unsupported platform
#endif

#ifndef APPLICATION_ID
	#define APPLICATION_ID "IzEngine"
#endif

#ifndef APPLICATION_VERSION
	#define APPLICATION_VERSION "undefined"
#endif

#ifndef CMAKE_BINARY_DIR
	#define CMAKE_BINARY_DIR ""
#endif

#define IZ_EXPAND(x) x
#define IZ_STRINGIFY(x) #x
#define IZ_C extern "C"
#define IZ_CPP extern "C++"
#define IZ_ASM extern

#define PLUGIN IZ_C EXPORT
#define ENTRY IZ_C EXPORT
#define BIT(n) (1U << (n))

#ifndef API
	#ifdef ENABLE_API
		#define API EXPORT
	#else
		#define API
	#endif
#endif

#ifdef _DEBUG
	#if defined(PLATFORM_WINDOWS)
		#define IZ_DEBUGBREAK() __debugbreak()
	#elif defined(PLATFORM_LINUX)
		#include <signal.h>
		#define IZ_DEBUGBREAK() raise(SIGTRAP)
	#else
		#define IZ_DEBUGBREAK()
	#endif
	#define IZ_ASSERT_ENABLED
#else
	#define IZ_DEBUGBREAK()
#endif

#ifdef IZ_ASSERT_ENABLED
	#define IZ_ASSERT_INTERNAL(check, msg, ...)               \
		if (!(check))                                         \
		{                                                     \
			Log::WriteLine(Channel::Error, msg, __VA_ARGS__); \
			IZ_DEBUGBREAK();                                  \
		}

	#define IZ_ASSERT_MESSAGE(check, ...) IZ_ASSERT_INTERNAL(check, "Assertion failed: {0}", __VA_ARGS__)
	#define IZ_ASSERT_VALUE(check) \
		IZ_ASSERT_INTERNAL(check, "Assertion '{0}' failed at {1}:{2}", IZ_STRINGIFY(check), __FILE__, __LINE__)

	#define IZ_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define IZ_ASSERT_GET_MACRO(...) \
		IZ_EXPAND(IZ_ASSERT_GET_MACRO_NAME(__VA_ARGS__, IZ_ASSERT_MESSAGE, IZ_ASSERT_VALUE))

	#define IZ_ASSERT(...) IZ_EXPAND(IZ_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__))
#else
	#define IZ_ASSERT(...)
#endif

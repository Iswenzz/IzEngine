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
	#define APPLICATION_ID "izengine"
#endif
#ifndef CMAKE_BINARY_DIR
	#define CMAKE_BINARY_DIR ""
#endif

#ifndef APPLICATION_VERSION
	#define APPLICATION_VERSION "undefined"
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

#include "Class.hpp"
#include "Debug.hpp"

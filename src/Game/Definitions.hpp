#pragma once

namespace IW3SR {};
using namespace IW3SR;

#ifndef CDECL
#define CDECL __cdecl
#endif

#ifndef OPTIMIZE3
#define OPTIMIZE3 __optimize3
#endif

#ifndef STDCALL
#define STDCALL __stdcall
#endif

#ifndef FASTCALL
#define FASTCALL __fastcall
#endif

#define C_EXTERN extern "C"
#define CPP_EXTERN extern "C++"
#define EXPORT C_EXTERN __declspec(dllexport)

#define CHECK_PARAMS(count, message)	\
if (Scr_GetNumParam() != count)			\
{										\
	Scr_Error(message);					\
	return;								\
}

#include "Definitions/Declaration.hpp"
#include "Definitions/Functions.hpp"
#include "Definitions/Structs.hpp"

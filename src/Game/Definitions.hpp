#pragma once

namespace IW3SR {};
using namespace IW3SR;

#define C_EXTERN extern "C"
#define OPTIMIZE3 __optimize3
#define CDECL __cdecl
#define STDCALL __stdcall

#define CHECK_PARAMS(count, message)	\
if (Scr_GetNumParam() != count)			\
{										\
	Scr_Error(message);					\
	return;								\
}

#include "Definitions/Declaration.hpp"
#include "Definitions/Functions.hpp"
#include "Definitions/Structs.hpp"

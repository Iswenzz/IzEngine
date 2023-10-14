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

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define FONT_SMALL_DEV  "fonts/smallDevFont"
#define FONT_BIG_DEV    "fonts/bigDevFont"
#define FONT_CONSOLE    "fonts/consoleFont"
#define FONT_BIG        "fonts/bigFont"
#define FONT_SMALL      "fonts/smallFont"
#define FONT_BOLD       "fonts/boldFont"
#define FONT_NORMAL     "fonts/normalFont"
#define FONT_EXTRA_BIG  "fonts/extraBigFont"
#define FONT_OBJECTIVE  "fonts/objectiveFont"

#include "Definitions/Declaration.hpp"
#include "Definitions/Functions.hpp"
#include "Definitions/Structs.hpp"

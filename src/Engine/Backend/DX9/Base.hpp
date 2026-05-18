#pragma once
#include "Engine/Core/Common.hpp"

#ifdef PLATFORM_WINDOWS
	#include "Engine/Platform/Windows/Base.hpp"
#endif

#pragma warning(push)
#pragma warning(disable : 26495)

#include <d3d9.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#undef DrawText

#pragma warning(pop)

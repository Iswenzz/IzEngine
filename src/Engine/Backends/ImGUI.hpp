#pragma once
#include "API.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <imgui_markdown.h>
#include <imgui_memory_editor.h>
#include <implot.h>
#include <implot_internal.h>

#ifdef WIN32
	#include <imgui_impl_win32.h>
#endif

#ifdef DIRECT3D_VERSION
	#if (DIRECT3D_VERSION == 0x900)
		#include <imgui_impl_dx9.h>
	#endif
#endif

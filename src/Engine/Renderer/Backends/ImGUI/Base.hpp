#pragma once
#include "Renderer/Common.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <imgui_markdown.h>
#include <imgui_memory_editor.h>
#include <implot.h>
#include <implot_internal.h>

#ifdef PLATFORM_WINDOWS
	#include "Windows/Base.hpp"
	#include <imgui_impl_win32.h>
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

#ifdef GRAPHICS_API_DX9
 	#include <imgui_impl_dx9.h>
#endif

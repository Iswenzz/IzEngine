#pragma once
#include "Engine/Base.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS

#pragma warning(push)
#pragma warning(disable : 26495)

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_markdown.h>
#include <imgui_memory_editor.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <implot_internal.h>

#ifdef PLATFORM_WINDOWS
	#include <imgui_impl_win32.h>
	#include "Windows/Base.hpp"

	#define ImGui_ImplOS_Init(handle) ImGui_ImplWin32_Init(handle)
	#define ImGui_ImplOS_Shutdown() ImGui_ImplWin32_Shutdown()
	#define ImGui_ImplOS_NewFrame() ImGui_ImplWin32_NewFrame()

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

#ifdef GRAPHICS_API_DX9
	#include <imgui_impl_dx9.h>

	#define ImGui_ImplAPI_Init(device) ImGui_ImplDX9_Init(device)
	#define ImGui_ImplAPI_Shutdown() ImGui_ImplDX9_Shutdown()
	#define ImGui_ImplAPI_InvalidateDeviceObjects() ImGui_ImplDX9_InvalidateDeviceObjects()
	#define ImGui_ImplAPI_CreateDeviceObjects() ImGui_ImplDX9_CreateDeviceObjects()
	#define ImGui_ImplAPI_NewFrame() ImGui_ImplDX9_NewFrame()
	#define ImGui_ImplAPI_RenderDrawData(data) ImGui_ImplDX9_RenderDrawData(data)
#endif

#pragma warning(pop)

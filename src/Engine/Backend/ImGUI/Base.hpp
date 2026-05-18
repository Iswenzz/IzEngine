#pragma once
#include "Engine/Core/Common.hpp"
#include "Engine/Renderer/UI/VirtualScreen.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS

#pragma warning(push)
#pragma warning(disable : 26495)

#define IM_VEC2_CLASS_EXTRA                              \
	constexpr ImVec2(const vec2& v) : x(v.x), y(v.y) { } \
	ImVec2(vec2& v) : x(v.x), y(v.y) { }                 \
	operator vec2() const                                \
	{                                                    \
		return vec2(x, y);                               \
	}

#define IM_VEC4_CLASS_EXTRA                                              \
	constexpr ImVec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { } \
	ImVec4(vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }                 \
	operator vec4() const                                                \
	{                                                                    \
		return vec4(x, y, z, w);                                         \
	}

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_markdown.h>
#include <imgui_memory_editor.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <implot_internal.h>

#ifdef PLATFORM_WINDOWS
	#include <imgui_impl_win32.h>
	#include "Engine/Platform/Windows/Base.hpp"

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

SERIALIZE_NON_INTRUSIVE(ImVec2, x, y)
SERIALIZE_NON_INTRUSIVE(ImVec4, x, y, z, w)

SERIALIZE_NON_INTRUSIVE(ImGuiStyle, Alpha, DisabledAlpha, WindowPadding, WindowRounding, WindowBorderSize,
	WindowMinSize, WindowTitleAlign, WindowMenuButtonPosition, ChildRounding, ChildBorderSize, PopupRounding,
	PopupBorderSize, FramePadding, FrameRounding, FrameBorderSize, ItemSpacing, ItemInnerSpacing, CellPadding,
	TouchExtraPadding, IndentSpacing, ColumnsMinSpacing, ScrollbarSize, ScrollbarRounding, GrabMinSize, GrabRounding,
	LogSliderDeadzone, TabRounding, TabBorderSize, ColorButtonPosition, ButtonTextAlign, SelectableTextAlign,
	SeparatorTextBorderSize, SeparatorTextAlign, SeparatorTextPadding, DisplayWindowPadding, DisplaySafeAreaPadding,
	MouseCursorScale, AntiAliasedLines, AntiAliasedLinesUseTex, AntiAliasedFill, CurveTessellationTol,
	CircleTessellationMaxError, Colors, HoverStationaryDelay, HoverDelayShort, HoverDelayNormal,
	HoverFlagsForTooltipMouse, HoverFlagsForTooltipNav)

SERIALIZE_NON_INTRUSIVE(ImPlotStyle, LineWeight, Marker, MarkerSize, MarkerWeight, FillAlpha, ErrorBarSize,
	ErrorBarWeight, DigitalBitHeight, DigitalBitGap, PlotBorderSize, MinorAlpha, MajorTickLen, MinorTickLen,
	MajorTickSize, MinorTickSize, MajorGridSize, MinorGridSize, PlotPadding, LabelPadding, LegendPadding,
	LegendInnerPadding, LegendSpacing, MousePosPadding, AnnotationPadding, FitPadding, PlotDefaultSize, PlotMinSize,
	Colors, Colormap, UseLocalTime, UseISO8601, Use24HourClock)

#pragma warning(pop)

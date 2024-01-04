#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include <d3dx9.h>
#include <wrl/client.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <imgui_stdlib.h>
#include <imgui_markdown.h>
#include <imgui_memory_editor.h>
#include <implot.h>
#include <implot_internal.h>
#include <nlohmann/json.hpp>

#include "Game/Definitions.hpp"
#include "Game/Render/Fonts/IconsFontAwesome6.hpp"
#include "Game/Render/Fonts/IconsFontAwesome6Brands.hpp"
#include "Utils/CircularBuffer.hpp"

#include "Math/Common.hpp"

using namespace Microsoft::WRL;

constexpr ImGuiWindowFlags ImGuiWindowFlags_Graph = ImGuiWindowFlags_NoTitleBar 
	| ImGuiWindowFlags_NoScrollbar 
	| ImGuiWindowFlags_NoCollapse 
	| ImGuiWindowFlags_NoBackground;

constexpr ImPlotAxisFlags ImPlotAxisFlags_Canvas = ImPlotAxisFlags_NoTickLabels
	| ImPlotAxisFlags_NoTickMarks
	| ImPlotAxisFlags_NoGridLines;

namespace ImGui
{
	/// <summary>
	/// Create a button.
	/// </summary>
	/// <param name="label">The button label.</param>
	/// <param name="v">Toggle state.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	API bool Button(const std::string& label, bool* v, const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Create a button.
	/// </summary>
	/// <param name="label">The button label.</param>
	/// <param name="id">The unique id.</param>
	/// <param name="v">Toggle state.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	API bool Button(const std::string& label, const std::string& id, bool* v,
		const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Create a toggle button.
	/// </summary>
	/// <param name="label">The button label.</param>
	/// <param name="id">The unique id.</param>
	/// <param name="v">Toggle state.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	API bool ButtonToggle(const std::string& label, const std::string& id, bool* v,
		const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Toggle button.
	/// </summary>
	/// <param name="id">The ID.</param>
	/// <param name="v">Toggle state.</param>
	/// <return></returns>
	API bool Toggle(const std::string& id, float size, bool* v);

	/// <summary>
	/// Combo box.
	/// </summary>
	/// <param name="label">The label.</param>
	/// <param name="item">The selected item.</param>
	/// <param name="items">The items.</param>
	/// <param name="maxHeightInItems">Max height in items popup.</param>
	/// <returns></returns>
	API bool Combo(const std::string& label, int* item, const std::vector<std::string>& items,
		int maxHeightInItems = -1);

	/// <summary>
	/// Creates a tooltip.
	/// </summary>
	/// <param name="text">The tooltip text.</param>
	/// <returns></returns>
	API void Tooltip(const std::string& text);

	/// <summary>
	/// Keybind component.
	/// </summary>
	/// <param name="label">The description.</param>
	/// <param name="key">The out key.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	API void Keybind(const std::string& label, int* key, const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Movable frame.
	/// </summary>
	/// <param name="label">The label.</param>
	/// <param name="position">The position.</param>
	/// <param name="size">The size.</param>
	/// <param name="renderPosition">The render position.</param>
	/// <param name="renderSize">The render size.</param>
	/// <returns></returns>
	API void Movable(const std::string& label, vec2& position, vec2& size, vec2& renderPosition, vec2& renderSize);

	/// <summary>
	/// Rainbow rect.
	/// </summary>
	/// <param name="position">The position.</param>
	/// <param name="size">The size.</param>
	/// <returns></returns>
	API void Rainbow(const vec2& position, const vec2& size);

	/// <summary>
	/// Render markdown.
	/// </summary>
	/// <param name="markdown">The markdown text.</param>
	/// <returns></returns>
	API void Markdown(const std::string& markdown);

	/// <summary>
	/// Is window resizing.
	/// </summary>
	/// <returns></returns>
	API bool IsWindowResizing();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

NLOHMANN_SERIALIZE_NON_INTRUSIVE(ImVec2, x, y)
NLOHMANN_SERIALIZE_NON_INTRUSIVE(ImVec4, x, y, z, w)

NLOHMANN_SERIALIZE_NON_INTRUSIVE(ImGuiStyle, Alpha, DisabledAlpha, WindowPadding, WindowRounding,
	WindowBorderSize, WindowMinSize, WindowTitleAlign, WindowMenuButtonPosition, ChildRounding,
	ChildBorderSize, PopupRounding, PopupBorderSize, FramePadding, FrameRounding, FrameBorderSize,
	ItemSpacing, ItemInnerSpacing, CellPadding, TouchExtraPadding, IndentSpacing, ColumnsMinSpacing,
	ScrollbarSize, ScrollbarRounding, GrabMinSize, GrabRounding, LogSliderDeadzone, TabRounding,
	TabBorderSize, TabMinWidthForCloseButton, ColorButtonPosition, ButtonTextAlign, SelectableTextAlign,
	SeparatorTextBorderSize, SeparatorTextAlign, SeparatorTextPadding, DisplayWindowPadding,
	DisplaySafeAreaPadding, MouseCursorScale, AntiAliasedLines, AntiAliasedLinesUseTex, AntiAliasedFill,
	CurveTessellationTol, CircleTessellationMaxError, Colors, HoverStationaryDelay, HoverDelayShort,
	HoverDelayNormal, HoverFlagsForTooltipMouse, HoverFlagsForTooltipNav)

NLOHMANN_SERIALIZE_NON_INTRUSIVE(ImPlotStyle, LineWeight, Marker, MarkerSize, MarkerWeight, FillAlpha,
	ErrorBarSize, ErrorBarWeight, DigitalBitHeight, DigitalBitGap, PlotBorderSize, MinorAlpha, MajorTickLen,
	MinorTickLen, MajorTickSize, MinorTickSize, MajorGridSize, MinorGridSize, PlotPadding, LabelPadding,
	LegendPadding, LegendInnerPadding, LegendSpacing, MousePosPadding, AnnotationPadding, FitPadding,
	PlotDefaultSize, PlotMinSize, Colors, Colormap, UseLocalTime, UseISO8601, Use24HourClock)

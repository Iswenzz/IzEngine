#pragma once
#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Core/Containers/CircularBuffer.hpp"
#include "Engine/Core/Math.hpp"

#include "Fonts/IconsFontAwesome6.hpp"
#include "Fonts/IconsFontAwesome6Brands.hpp"

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
	/// param name="size">The button size.</param>
	/// <return></returns>
	API bool Toggle(const std::string& id, bool* v, const ImVec2& size = ImVec2(0, 0));

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

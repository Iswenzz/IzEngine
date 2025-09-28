#pragma once
#include "ImGUI/Base.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Containers/CircularBuffer.hpp"
#include "Core/Math.hpp"

#include "Fonts/IconsFontAwesome6.hpp"
#include "Fonts/IconsFontAwesome6Brands.hpp"

constexpr ImGuiWindowFlags ImGuiWindowFlags_Widget = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
	| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

constexpr ImGuiWindowFlags ImGuiWindowFlags_Notification = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs
	| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;

constexpr ImPlotAxisFlags ImPlotAxisFlags_Canvas =
	ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoGridLines;

constexpr std::array<const char*, 3> Horizontals = { "Left", "Center", "Right" };
constexpr std::array<const char*, 3> Verticals = { "Top", "Center", "Bottom" };

namespace ImGui
{
	API extern MarkdownConfig MarkConfig;
	API extern std::tuple<ImColor, ImColor> RainbowTuple;

	API extern ImFont* H1;
	API extern ImFont* H2;
	API extern ImFont* H3;

	/// <summary>
	/// Push ID.
	/// </summary>
	/// <param name="uuid">The UUID.</param>
	/// <returns></returns>
	API void PushID(const UUID& uuid);

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
	API bool Button(const std::string& label, const std::string& id, bool* v, const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Create a toggle button.
	/// </summary>
	/// <param name="label">The button label.</param>
	/// <param name="id">The unique id.</param>
	/// <param name="v">Toggle state.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	API bool ButtonToggle(const std::string& label, const std::string& id, bool* v, const ImVec2& size = ImVec2(0, 0));

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
	/// Alignments combo.
	/// </summary>
	/// <param name="x">The x alignment.</param>
	/// <param name="y">The y alignment.</param>
	/// <returns></returns>
	API bool ComboAlign(Alignment* x, Alignment* y);

	/// <summary>
	/// Rect alignments combo.
	/// </summary>
	/// <param name="horizontal">The horizontal alignment.</param>
	/// <param name="vertical">The vertical alignment.</param>
	/// <returns></returns>
	API bool ComboAlignRect(Horizontal* horizontal, Vertical* vertical);

	/// <summary>
	/// Collapsing header.
	/// </summary>
	/// <param name="label">The label.</param>
	/// <param name="open">Is default open.</param>
	/// <param name="flags">The tree node flags.</param>
	/// <returns></returns>
	API bool CollapsingHeader(const std::string& label, bool open = false, ImGuiTreeNodeFlags flags = 0);

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
	/// <param name="unbind">Can unbind with right click.</param>
	/// <param name="size">The button size.</param>
	/// <returns></returns>
	API void Keybind(const std::string& label, InputEnum* key, bool unbind = true, const ImVec2& size = ImVec2(0, 0));

	/// <summary>
	/// Movable frame.
	/// </summary>
	/// <param name="id">The ID.</param>
	/// <param name="position">The position.</param>
	/// <param name="size">The size.</param>
	/// <param name="renderPosition">The render position.</param>
	/// <param name="renderSize">The render size.</param>
	/// <returns></returns>
	API void Movable(const UUID& id, vec2& position, vec2& size, vec2& renderPosition, vec2& renderSize);

	/// <summary>
	/// Compute rainbow animation.
	/// </summary>
	/// <returns></returns>
	API void ComputeRainbow();

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
	/// Loading indicator.
	/// </summary>
	/// <param name="label">The label.</param>
	/// <param name="color">The color.</param>
	/// <param name="state">The visible state.</param>
	API void LoadingIndicator(const std::string& label, const ImVec2& pos, const ImU32& color, bool state);

	/// <summary>
	/// Is resizing.
	/// </summary>
	/// <returns></returns>
	API bool IsResizing();

	/// <summary>
	/// Is moving.
	/// </summary>
	/// <returns></returns>
	API bool IsMoving();
}

SERIALIZE_NON_INTRUSIVE(ImVec2, x, y)
SERIALIZE_NON_INTRUSIVE(ImVec4, x, y, z, w)

SERIALIZE_NON_INTRUSIVE(ImGuiStyle, Alpha, DisabledAlpha, WindowPadding, WindowRounding, WindowBorderSize,
	WindowMinSize, WindowTitleAlign, WindowMenuButtonPosition, ChildRounding, ChildBorderSize, PopupRounding,
	PopupBorderSize, FramePadding, FrameRounding, FrameBorderSize, ItemSpacing, ItemInnerSpacing, CellPadding,
	TouchExtraPadding, IndentSpacing, ColumnsMinSpacing, ScrollbarSize, ScrollbarRounding, GrabMinSize, GrabRounding,
	LogSliderDeadzone, TabRounding, TabBorderSize, ColorButtonPosition, ButtonTextAlign,
	SelectableTextAlign, SeparatorTextBorderSize, SeparatorTextAlign, SeparatorTextPadding, DisplayWindowPadding,
	DisplaySafeAreaPadding, MouseCursorScale, AntiAliasedLines, AntiAliasedLinesUseTex, AntiAliasedFill,
	CurveTessellationTol, CircleTessellationMaxError, Colors, HoverStationaryDelay, HoverDelayShort, HoverDelayNormal,
	HoverFlagsForTooltipMouse, HoverFlagsForTooltipNav)

SERIALIZE_NON_INTRUSIVE(ImPlotStyle, LineWeight, Marker, MarkerSize, MarkerWeight, FillAlpha, ErrorBarSize,
	ErrorBarWeight, DigitalBitHeight, DigitalBitGap, PlotBorderSize, MinorAlpha, MajorTickLen, MinorTickLen,
	MajorTickSize, MinorTickSize, MajorGridSize, MinorGridSize, PlotPadding, LabelPadding, LegendPadding,
	LegendInnerPadding, LegendSpacing, MousePosPadding, AnnotationPadding, FitPadding, PlotDefaultSize, PlotMinSize,
	Colors, Colormap, UseLocalTime, UseISO8601, Use24HourClock)

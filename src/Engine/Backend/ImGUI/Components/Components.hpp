#pragma once
#include "Engine/Backend/ImGUI/Base.hpp"
#include "Engine/Backend/ImGUI/Fonts/IconsFontAwesome6.hpp"
#include "Engine/Backend/ImGUI/Fonts/IconsFontAwesome6Brands.hpp"

#include "Engine/Core/Containers/CircularBuffer.hpp"
#include "Engine/Core/Input/Input.hpp"

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

	API void PushID(const UUID& uuid);
	API bool Button(const std::string& label, bool* v, const vec2& size = vec2(0, 0));
	API bool Button(const std::string& label, const std::string& id, bool* v, const vec2& size = vec2(0, 0));
	API bool ButtonToggle(const std::string& label, const std::string& id, bool* v, const vec2& size = vec2(0, 0));
	API bool Toggle(const std::string& id, bool* v, const vec2& size = vec2(0, 0));
	API bool Combo(const std::string& label, int* item, const std::vector<std::string>& items, int maxHeight = -1);
	API bool ComboAlign(Alignment* x, Alignment* y);
	API bool ComboAlignRect(Horizontal* horizontal, Vertical* vertical);
	API bool CollapsingHeader(const std::string& label, bool open = false, ImGuiTreeNodeFlags flags = 0);
	API void Tooltip(const std::string& text);
	API void Keybind(const std::string& label, InputEnum* key, bool unbind = true, const vec2& size = vec2(0, 0));
	API void Movable(const UUID& id, vec2& position, vec2& size, vec2& renderPosition, vec2& renderSize);
	API void ComputeRainbow();
	API void Rainbow(const vec2& position, const vec2& size);
	API void Markdown(const std::string& markdown);
	API void LoadingIndicator(const std::string& label, const vec2& pos, const ImU32& color, bool state);
	API bool IsResizing();
	API bool IsMoving();
}

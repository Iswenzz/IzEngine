#include "Themes.hpp"

namespace IW3SR::UI
{
	Themes::Themes() : Window("Themes")
	{
		Default();
	}
	
	void Themes::Initialize()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;

		ImGuiStyle& style = ImGui::GetStyle();
		style = Style;

		const float fontSize = 22;
		const float iconSize = fontSize * 2.f / 3.f;

		static const ImWchar rangesFa[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		static const ImWchar rangesFab[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };

		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH = true;
		config.GlyphMinAdvanceX = iconSize;

		const auto openSans = Environment::FontsDirectory / "OpenSans-Regular.ttf";
		const auto faRegular = Environment::FontsDirectory / "fa-regular-400.ttf";
		const auto faSolid = Environment::FontsDirectory / "fa-solid-900.ttf";
		const auto faBrands = Environment::FontsDirectory / "fa-brands-400.ttf";

		io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize);
		io.Fonts->AddFontFromFileTTF(faRegular.string().c_str(), iconSize, &config, rangesFa);
		io.Fonts->AddFontFromFileTTF(faSolid.string().c_str(), iconSize, &config, rangesFa);
		io.Fonts->AddFontFromFileTTF(faBrands.string().c_str(), iconSize, &config, rangesFab);
	}

	void Themes::Default()
	{
		ImGuiStyle& style = Style;
		ImVec4* colors = style.Colors;

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.52f, 0.54f, 0.59f, 0.80f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.11f, 0.71f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.12f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.19f, 0.20f, 0.60f);
		colors[ImGuiCol_Border] = ImVec4(0.15f, 0.17f, 0.19f, 0.29f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.78f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.16f, 0.20f, 0.54f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.24f, 0.27f, 0.70f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.49f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.40f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.20f, 0.23f, 0.79f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.22f, 0.24f, 0.29f, 0.84f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.38f, 0.42f, 0.51f, 0.54f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.34f, 0.16f, 0.90f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.34f, 0.43f, 0.45f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.32f, 0.38f, 0.45f, 0.54f);
		colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.16f, 0.19f, 0.70f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.16f, 0.90f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.19f, 0.21f, 0.24f, 0.33f);
		colors[ImGuiCol_Separator] = ImVec4(0.23f, 0.25f, 0.30f, 0.29f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.38f, 0.43f, 0.29f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.42f, 0.47f, 0.55f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.32f, 0.33f, 0.36f, 0.29f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.41f, 0.45f, 0.51f, 0.29f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.36f, 0.40f, 0.46f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.27f, 0.33f, 0.36f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.28f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.23f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.32f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.25f, 0.30f, 0.29f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.08f, 0.41f, 0.97f, 0.59f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.30f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.37f, 0.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.28f, 0.00f, 1.00f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.35f, 0.00f, 1.00f, 0.35f);

		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.WindowPadding = ImVec2(10, 10);
		style.FramePadding = ImVec2(8, 8);
		style.CellPadding = ImVec2(6, 6);
		style.ItemSpacing = ImVec2(6, 6);
		style.ItemInnerSpacing = ImVec2(6, 6);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 25;
		style.ScrollbarSize = 16;
		style.GrabMinSize = 12;
		style.WindowBorderSize = 0;
		style.ChildBorderSize = 0;
		style.PopupBorderSize = 0;
		style.FrameBorderSize = 0;
		style.TabBorderSize = 0;
		style.SeparatorTextBorderSize = 1;
		style.WindowRounding = 0;
		style.ChildRounding = 0;
		style.FrameRounding = 0;
		style.PopupRounding = 0;
		style.ScrollbarRounding = 12;
		style.GrabRounding = 0;
		style.LogSliderDeadzone = 4;
		style.TabRounding = 0;
	}

	void Themes::ComputeRainbow()
	{
		const float speed = 0.15f;
		static float offset = 0;

		Rainbow = std::make_tuple(
			ImColor::HSV(fmod(offset, 1.0f), 1.0f, 1.0f),
			ImColor::HSV(fmod(offset + 0.33f, 1.0f), 1.0f, 1.0f)
		);
		offset += speed * ImGui::GetIO().DeltaTime;
	}

	void Themes::Frame()
	{
		if (!Open) return;

		Style = ImGui::GetStyle();

		Begin();
		ImGui::Separator();
		ImGui::ShowStyleEditor();

		if (ImGui::Button("Default"))
			Default();
		End();
	}
}

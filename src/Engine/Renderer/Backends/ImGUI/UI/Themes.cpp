#include "Themes.hpp"

#include "ImGUI/UI.hpp"
#include "Core/System/Environment.hpp"
#include "Core/System/System.hpp"

namespace IW3SR::Engine::UC
{
	Themes::Themes() : Window("Themes")
	{
		Default();
	}

	void Themes::Initialize()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;

		ImGui::GetStyle() = Style;
		ImPlot::GetStyle() = PlotStyle;

		const float fontSize = 12.f * UI::Get().Size;
		const float iconSize = 8.f * UI::Get().Size;

		static const ImWchar rangesFa[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		static const ImWchar rangesFab[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };

		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH = true;
		config.OversampleH = true;
		config.GlyphMinAdvanceX = iconSize;

		const auto openSans = Environment::FontsDirectory / "OpenSans-Regular.ttf";
		const auto faRegular = Environment::FontsDirectory / "fa-regular-400.ttf";
		const auto faSolid = Environment::FontsDirectory / "fa-solid-900.ttf";
		const auto faBrands = Environment::FontsDirectory / "fa-brands-400.ttf";

		io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize);
		io.Fonts->AddFontFromFileTTF(faRegular.string().c_str(), iconSize, &config, rangesFa);
		io.Fonts->AddFontFromFileTTF(faSolid.string().c_str(), iconSize, &config, rangesFa);
		io.Fonts->AddFontFromFileTTF(faBrands.string().c_str(), iconSize, &config, rangesFab);

		H1 = io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize * 1.5);
		H2 = io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize * 1.25);
		H3 = io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize * 1.125);

		Markdown.linkIcon = ICON_FA_LINK;
		Markdown.linkCallback = MarkdownLink;
		Markdown.imageCallback = MarkdownImage;
		Markdown.formatCallback = MarkdownFormat;
		Markdown.tooltipCallback = nullptr;
		Markdown.headingFormats[0] = { H1, true };
		Markdown.headingFormats[1] = { H2, true };
		Markdown.headingFormats[2] = { H3, false };
		Markdown.userData = nullptr;
	}

	void Themes::Default()
	{
		ImVec4* colors = Style.Colors;
		ImVec4* plotColors = PlotStyle.Colors;

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

		plotColors[ImPlotCol_Line] = ImVec4(0.34f, 0.16f, 0.90f, 1.00f);
		plotColors[ImPlotCol_Fill] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_MarkerOutline] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_MarkerFill] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_ErrorBar] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.00f);
		plotColors[ImPlotCol_PlotBg] = ImVec4(0.09f, 0.09f, 0.11f, 0.71f);
		plotColors[ImPlotCol_PlotBorder] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_LegendBg] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_LegendBorder] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_LegendText] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_TitleText] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_InlayText] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_AxisText] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_AxisGrid] = ImVec4(0.25, 0.58f, 0.58f, 0.58f);
		plotColors[ImPlotCol_AxisTick] = ImVec4(0.25, 0.54f, 0.54f, 0.54f);
		plotColors[ImPlotCol_AxisBg] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_AxisBgHovered] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_AxisBgActive] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_Selection] = IMPLOT_AUTO_COL;
		plotColors[ImPlotCol_Crosshairs] = IMPLOT_AUTO_COL;

		Style.WindowMenuButtonPosition = ImGuiDir_None;
		Style.WindowPadding = ImVec2(10, 10);
		Style.FramePadding = ImVec2(8, 8);
		Style.CellPadding = ImVec2(6, 6);
		Style.ItemSpacing = ImVec2(8, 8);
		Style.ItemInnerSpacing = ImVec2(10, 10);
		Style.TouchExtraPadding = ImVec2(0, 0);
		Style.IndentSpacing = 25;
		Style.ScrollbarSize = 16;
		Style.GrabMinSize = 12;
		Style.WindowBorderSize = 0;
		Style.ChildBorderSize = 0;
		Style.PopupBorderSize = 0;
		Style.FrameBorderSize = 0;
		Style.TabBorderSize = 0;
		Style.SeparatorTextBorderSize = 1;
		Style.WindowRounding = 0;
		Style.ChildRounding = 0;
		Style.FrameRounding = 0;
		Style.PopupRounding = 0;
		Style.ScrollbarRounding = 12;
		Style.GrabRounding = 0;
		Style.LogSliderDeadzone = 4;
		Style.TabRounding = 0;

		PlotStyle.AnnotationPadding = ImVec2(2, 2);
		PlotStyle.Colormap = 0;
		PlotStyle.DigitalBitGap = 4;
		PlotStyle.DigitalBitHeight = 8;
		PlotStyle.ErrorBarSize = 5;
		PlotStyle.ErrorBarWeight = 1.5;
		PlotStyle.FillAlpha = 1;
		PlotStyle.FitPadding = ImVec2(0, 0);
		PlotStyle.LabelPadding = ImVec2(5, 5);
		PlotStyle.LegendInnerPadding = ImVec2(5, 5);
		PlotStyle.LegendPadding = ImVec2(10, 10);
		PlotStyle.LegendSpacing = ImVec2(5, 0);
		PlotStyle.LineWeight = 2;
		PlotStyle.MajorGridSize = ImVec2(1, 1);
		PlotStyle.MajorTickLen = ImVec2(10, 10);
		PlotStyle.MajorTickSize = ImVec2(1, 1);
		PlotStyle.Marker = -1;
		PlotStyle.MarkerSize = 4;
		PlotStyle.MarkerWeight = 1;
		PlotStyle.MinorAlpha = 0;
		PlotStyle.MinorGridSize = ImVec2(1, 1);
		PlotStyle.MinorTickLen = ImVec2(18, 10);
		PlotStyle.MinorTickSize = ImVec2(1, 1);
		PlotStyle.MousePosPadding = ImVec2(18, 10);
		PlotStyle.PlotBorderSize = 0;
		PlotStyle.PlotDefaultSize = ImVec2(400, 250);
		PlotStyle.PlotMinSize = ImVec2(200, 150);
		PlotStyle.PlotPadding = ImVec2(0, 0);
		PlotStyle.Use24HourClock = false;
		PlotStyle.UseISO8601 = false;
		PlotStyle.UseLocalTime = false;

		Apply();
	}

	void Themes::Apply()
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImGui::GetStyle() = Style;
		ImPlot::GetStyle() = PlotStyle;
	}

	void Themes::ComputeRainbow()
	{
		const float speed = 0.15f;
		static float offset = 0;

		Rainbow = std::make_tuple(
			ImColor::HSV(fmod(offset, 1.0f), 1.0f, 1.0f),
			ImColor::HSV(fmod(offset + 0.33f, 1.0f), 1.0f, 1.0f)
		);
		offset += speed * UI::Get().DeltaTime();
	}

	void Themes::MarkdownLink(ImGui::MarkdownLinkCallbackData data)
	{
		std::string url(data.link, data.linkLength);
		if (!data.isImage) System::Shell(url);
	}

	ImGui::MarkdownImageData Themes::MarkdownImage(ImGui::MarkdownLinkCallbackData data)
	{
		ImTextureID image = ImGui::GetIO().Fonts->TexID;
		ImGui::MarkdownImageData imageData;
		imageData.isValid = true;
		imageData.useLinkCallback = false;
		imageData.user_texture_id = image;
		imageData.size = ImVec2(40.0f, 20.0f);

		ImVec2 const contentSize = ImGui::GetContentRegionAvail();
		if (imageData.size.x > contentSize.x)
		{
			const float ratio = imageData.size.y / imageData.size.x;
			imageData.size.x = contentSize.x;
			imageData.size.y = contentSize.x * ratio;
		}
		return imageData;
	}

	void Themes::MarkdownFormat(const ImGui::MarkdownFormatInfo& info, bool start)
	{
		ImGui::defaultMarkdownFormatCallback(info, start);

		switch (info.type)
		{
		case ImGui::MarkdownFormatType::LINK:
			start
				? ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive])
				: ImGui::PopStyleColor();
			break;
		}
	}

	void Themes::Render()
	{
		ComputeRainbow();

		if (!Open) return;

		Style = ImGui::GetStyle();
		PlotStyle = ImPlot::GetStyle();

		Begin();
		if (ImGui::Button("Default Theme"))
			Default();

		ImGui::Separator();
		ImGui::ShowStyleEditor();

		ImGui::Separator();
		ImPlot::ShowStyleEditor();
		End();
	}
}

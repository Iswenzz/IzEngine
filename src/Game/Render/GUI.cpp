#include "GUI.hpp"
#include "ImGUI.hpp"
#include "Game/Game.hpp"

#include "Sys/Environment.hpp"
#include "Sys/Fonts/IconsFontAwesome6.hpp"
#include "Sys/Fonts/IconsFontAwesome6Brands.hpp"

namespace IW3SR
{
	GUI::GUI()
	{
		if (COD4X)
		{
			MainWndProc_h.Address = Memory::Scan("cod4x_021",
				"\x55\x89\xE5\x53\x81\xEC\x84\x00\x00\x00\xC7\x04\x24\x02", 14);
		}
		CreateWindowExA_h.Install();
		MainWndProc_h.Install();
	}

	void GUI::Initialize()
	{
		if (Active) return;

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(MainWindow);
		ImGui_ImplDX9_Init(*dx9_device);
		Theme();

		Active = true;
	}

	void GUI::Theme()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		const float fontSize = 18;
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

		colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.5, 0.5, 0.5, 1);
		colors[ImGuiCol_WindowBg] = ImVec4(0.1, 0.1, 0.1, 0.7);
		colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
		colors[ImGuiCol_PopupBg] = ImVec4(0.19, 0.19, 0.19, 0.6);
		colors[ImGuiCol_Border] = ImVec4(0.19, 0.19, 0.19, 0.29);
		colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0.24);
		colors[ImGuiCol_FrameBg] = ImVec4(0.05, 0.05, 0.05, 0.54);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19, 0.19, 0.19, 0.54);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.2, 0.22, 0.23, 0.7);
		colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.06, 0.06, 0.06, 1);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14, 0.14, 0.14, 1);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05, 0.05, 0.05, 0.54);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34, 0.34, 0.34, 0.54);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4, 0.4, 0.4, 0.54);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56, 0.56, 0.56, 0.54);
		colors[ImGuiCol_CheckMark] = ImVec4(0.33, 0.67, 0.86, 1);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.34, 0.34, 0.34, 0.54);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56, 0.56, 0.56, 0.54);
		colors[ImGuiCol_Button] = ImVec4(0.05, 0.05, 0.05, 0.54);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.19, 0.19, 0.19, 0.7);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.275, 0, 1., 1);
		colors[ImGuiCol_Header] = ImVec4(0, 0, 0, 0.52);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0, 0, 0, 0.36);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.2, 0.22, 0.23, 0.33);
		colors[ImGuiCol_Separator] = ImVec4(0.28, 0.28, 0.28, 0.29);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44, 0.44, 0.44, 0.29);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.4, 0.44, 0.47, 1);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.28, 0.28, 0.28, 0.29);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44, 0.44, 0.44, 0.29);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4, 0.44, 0.47, 1);
		colors[ImGuiCol_Tab] = ImVec4(0, 0, 0, 0.52);
		colors[ImGuiCol_TabHovered] = ImVec4(0.14, 0.14, 0.14, 1);
		colors[ImGuiCol_TabActive] = ImVec4(0.2, 0.2, 0.2, 0.36);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0, 0, 0, 0.52);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14, 0.14, 0.14, 1);
		colors[ImGuiCol_PlotLines] = ImVec4(1, 0, 0, 1);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1, 0, 0, 1);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1, 0, 0, 1);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1, 0, 0, 1);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0, 0, 0, 0.52);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0, 0, 0, 0.52);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.28, 0.28, 0.28, 0.29);
		colors[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1, 1, 1, 0.06);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2, 0.22, 0.23, 1);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.33, 0.67, 0.86, 1);
		colors[ImGuiCol_NavHighlight] = ImVec4(1, 0, 0, 1);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 0, 0, 0.7);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1, 0, 0, 0.2);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1, 0, 0, 0.35);

		style.WindowPadding = ImVec2(8, 8);
		style.FramePadding = ImVec2(6, 6);
		style.CellPadding = ImVec2(6, 6);
		style.ItemSpacing = ImVec2(6, 6);
		style.ItemInnerSpacing = ImVec2(6, 6);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 25;
		style.ScrollbarSize = 15;
		style.GrabMinSize = 10;
		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 1;
		style.TabBorderSize = 1;
		style.WindowRounding = 0;
		style.ChildRounding = 0;
		style.FrameRounding = 0;
		style.PopupRounding = 0;
		style.ScrollbarRounding = 9;
		style.GrabRounding = 3;
		style.LogSliderDeadzone = 4;
		style.TabRounding = 0;
	}

	void GUI::Reset()
	{
		if (!Active) return;

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ResetMouse();

		Active = false;
	}

	void GUI::ResetMouse()
	{
		dvar_s *mouse = Dvar_FindVar("in_mouse");
		if (!mouse) return;

		mouse->current.enabled = true;
		s_wmv->mouseInitialized = true;
	}

	void GUI::Begin()
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		Frame();
	}

	void GUI::End()
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	void GUI::Frame()
	{
		if (!Open) return;

		ImGui::Begin("Modules", &Open);
		ImGui::SetWindowSize({ 400, 300 }, ImGuiCond_FirstUseEver);
		const float frameWidth = ImGui::GetContentRegionAvail().x - 16;
		static bool v = false;

		for (const auto& [_, entry] : SR->Modules->Entries)
		{
			const char* name = entry->Name.c_str();
			ImGui::ToggleButton(entry->ID + "toggle", 20.f, &v);
			ImGui::SameLine();
			ImGui::Text(name);
			ImGui::SameLine(frameWidth);

			if (ImGui::ButtonId(ICON_FA_GEAR, entry->ID + "menu") || entry->MenuOpen)
			{
				entry->MenuOpen = true;
				ImGui::Begin(name, &entry->MenuOpen);
				ImGui::SetWindowSize(entry->MenuSize, ImGuiCond_FirstUseEver);
				entry->OnMenu();
				ImGui::End();
			}
		}
		ImGui::End();
	}

	HWND GUI::CreateMainWindow(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
		HINSTANCE hInstance, LPVOID lpParam)
	{
		HWND hwnd = CreateWindowExA_h(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y,
			nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

		std::string windowName = std::string{ lpWindowName };
		if (windowName != "Call of Duty 4" && windowName != "Call of Duty 4 X")
			return hwnd;

		Reset();
		MainWindow = hwnd;
		return MainWindow;
	}

	LRESULT GUI::MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		if (!Active)
			return MainWndProc_h(hWnd, Msg, wParam, lParam);

		if (GetAsyncKeyState(VK_F10))
			Open = !Open;

		ImGuiIO& io = ImGui::GetIO();
		if (Open)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
			io.MouseDrawCursor = true;
			return true;
		}
		io.MouseDrawCursor = false;
		return MainWndProc_h(hWnd, Msg, wParam, lParam);
	}
}

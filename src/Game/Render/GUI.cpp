#include "GUI.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	GUI::GUI()
	{
		Patch();

		Toolbar = class Toolbar();
		About = class About();
		Binds = class Binds();
		Settings = class Settings();
		Themes = class Themes();

		OpenKey = KeyListener(VK_F10);
	}

	void GUI::Patch()
	{
		if (COD4X)
		{
			MainWndProc_h.Address = Memory::Scan(COD4X_BIN,
				"\x55\x89\xE5\x53\x81\xEC\x84\x00\x00\x00\xC7\x04\x24\x02", 14);
		}
	}

	void GUI::Initialize()
	{
		if (Active || !MainWindow) return;
		Active = true;

		ImGui::SetAllocatorFunctions(&Allocator, &Free, &Data);
		Context = ImGui::CreateContext();
		ImGui_ImplWin32_Init(MainWindow);
		ImGui_ImplDX9_Init(dx->device);
		Themes.Initialize();

		SR->Modules->SetRenderer();
	}

	void GUI::Shutdown()
	{
		if (!Active) return;
		Active = false;

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext(Context);
	}

	void GUI::Reset()
	{
		if (!Active) return;

		ResetMouse();
		Shutdown();
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
		if (OpenKey.IsPressed())
			Open = !Open;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Themes.ComputeRainbow();
		Frame();
	}

	void GUI::End()
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		KeyListener::Reset();
	}

	void GUI::Frame()
	{
		if (!Open) return;

		About.Frame();
		Binds.Frame();
		Settings.Frame();
		Themes.Frame();
		Toolbar.Frame();

		SR->Modules->Frame();
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
		KeyListener::Process(Msg, wParam, lParam);

		if (!Active)
			return MainWndProc_h(hWnd, Msg, wParam, lParam);

		ImGuiIO& io = ImGui::GetIO();
		if (Open)
		{
			if (GetAsyncKeyState(VK_ESCAPE))
				Open = false;
			ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
			io.MouseDrawCursor = true;
			return true;
		}
		io.MouseDrawCursor = false;
		return MainWndProc_h(hWnd, Msg, wParam, lParam);
	}

	void* GUI::Allocator(size_t size, void* data)
	{
		return malloc(size);
	}

	void GUI::Free(void* ptr, void* data)
	{
		free(ptr);
	}
}

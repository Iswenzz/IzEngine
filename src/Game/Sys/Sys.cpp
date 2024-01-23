#include "Sys.hpp"
#include "Game/Game.hpp"

namespace IW3SR::Game
{
	Sys::Sys()
	{
		Patch();
	}

	void Sys::Patch()
	{
		if (COD4X)
		{
			MainWndProc_h.Address = Memory::Scan(COD4X_BIN,
				"\x55\x89\xE5\x53\x81\xEC\x84\x00\x00\x00\xC7\x04\x24\x02", 14);
		}
	}

	HWND Sys::CreateMainWindow(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
		HINSTANCE hInstance, LPVOID lpParam)
	{
		HWND hwnd = CreateWindowExA_h(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y,
			nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

		std::string windowName = std::string{ lpWindowName };
		if (windowName != "Call of Duty 4" && windowName != "Call of Duty 4 X")
			return hwnd;

		return MainWindow = hwnd;
	}

	LRESULT Sys::MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		auto& UI = UI::Get();
		KeyListener::Process(Msg, wParam, lParam);

		if (!UI.Active)
			return MainWndProc_h(hWnd, Msg, wParam, lParam);

		ImGuiIO& io = ImGui::GetIO();
		if (UI.Open)
		{
			if (KeyListener::IsPressed(VK_ESCAPE))
				UI.Open = false;
			ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
			io.MouseDrawCursor = true;
			return true;
		}
		io.MouseDrawCursor = false;
		return MainWndProc_h(hWnd, Msg, wParam, lParam);
	}
}

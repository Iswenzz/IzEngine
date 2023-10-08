#include "GUI.hpp"
#include "Sys/Memory.hpp"

Hook<HWND STDCALL(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, 
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, 
	HINSTANCE hInstance, LPVOID lpParam)>
	CreateWindowExA_h((uintptr_t)CreateWindowExA, GUI::CreateMainWindow);

Hook<LRESULT CALLBACK(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)>
	MainWndProc_h(0x57BB20, GUI::MainWndProc);

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

		Active = true;
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
	}

	void GUI::End()
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
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

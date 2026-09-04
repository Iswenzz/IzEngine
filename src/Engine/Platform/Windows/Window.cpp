#include "Base.hpp"

#include "Engine/Core/System/Window.hpp"
#include "Engine/Renderer/Base/Renderer.hpp"

namespace IzEngine
{
	void Window::Initialize(const std::string& name)
	{
		HINSTANCE instance = GetModuleHandle(nullptr);

		const auto WndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{ return static_cast<LRESULT>(Window::Update(hWnd, uMsg, wParam, lParam)); };

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = "WindowClass";
		wc.hIconSm = LoadIcon(0, IDI_APPLICATION);
		RegisterClassEx(&wc);

		RECT screen;
		GetWindowRect(GetDesktopWindow(), &screen);
		Position = vec2((screen.right - Size.x) / 2, (screen.bottom - Size.y) / 2);

		DWORD style = WS_OVERLAPPEDWINDOW;
		HWND hwnd = CreateWindowEx(0, wc.lpszClassName, name.c_str(), style, Position.x, Position.y, Size.x, Size.y,
			nullptr, nullptr, instance, nullptr);

		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		Handle = hwnd;
		Open = Handle;

		Input::Initialize();
		Keyboard::Register();
		Mouse::Register();
	}

	void Window::Swap(void* handle)
	{
		Swapped = true;

		const HWND hwnd = reinterpret_cast<HWND>(handle);
		Handle = handle;
		Open = handle;

		RECT rect;
		GetWindowRect(hwnd, &rect);
		Position = vec2(rect.left, rect.top);
		GetClientRect(hwnd, &rect);
		Size = vec2(rect.right, rect.bottom);

		Input::Initialize();
		Keyboard::Register();
		Mouse::Register();
	}

	void Window::Shutdown()
	{
		if (Handle)
			DestroyWindow(reinterpret_cast<HWND>(Handle));
	}

	int Window::Update(void* handle, int msg, uintptr_t arg1, uintptr_t arg2)
	{
		const HWND hwnd = reinterpret_cast<HWND>(handle);
		const WPARAM wParam = arg1;
		const LPARAM lParam = arg2;

		switch (msg)
		{
		case WM_INPUT:
			Mouse::Process(msg, lParam);
			Keyboard::Process(msg, lParam);
			break;

		case WM_MOUSEMOVE:
			Mouse::Process(msg, lParam);
			break;

		case WM_CHAR:
			Keyboard::Process(msg, wParam);
			break;

		case WM_MOVE:
			Position = vec2(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_SIZE:
			Size = vec2(LOWORD(lParam), HIWORD(lParam));
			IsMinimized = wParam == SIZE_MINIMIZED;
			if (Renderer::Active && !IsMinimized)
				Renderer::Resize(Size);
			break;

		case WM_CLOSE:
			Open = false;
			UI::Open = false;
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		if (Intercept(hwnd, msg, wParam, lParam))
			return 0;
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool Window::Intercept(void* handle, int msg, uintptr_t arg1, uintptr_t arg2)
	{
		const HWND hwnd = reinterpret_cast<HWND>(handle);
		const WPARAM wParam = arg1;
		const LPARAM lParam = arg2;

		// The menu key belongs to the overlay, so its own key messages go no further. Only those:
		// dropping every message of the frame the key goes down also drops the releases ImGui needs,
		// and a modifier it never sees released stays down for good, which leaves the menu unable to
		// take text.
		switch (msg)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			if (UI::KeyOpen.Input != Input_None && Input::MapKey(static_cast<int>(wParam)) == UI::KeyOpen.Input)
				return true;
			break;
		}

		if (!UI::Open)
			return false;
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;

		// Alt and F10 send DefWindowProc into the system menu loop, and that loop swallows every
		// keystroke until another Alt, an Escape or a click dismisses it. There is no menu bar to
		// reach that way, and the menu key is F10 by default. Only while the overlay is open: closed,
		// the key is the host application's to bind.
		switch (msg)
		{
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			return wParam == VK_MENU || wParam == VK_F10;
		case WM_SYSCHAR:
			return true;
		case WM_SYSCOMMAND:
			return (wParam & 0xFFF0) == SC_KEYMENU || (wParam & 0xFFF0) == SC_MOUSEMENU;
		}
		return false;
	}

	bool Window::Frame()
	{
		const HWND hwnd = reinterpret_cast<HWND>(Handle);

		MSG msg = { 0 };
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (AppliedCapture != IsCapture)
		{
			SetWindowDisplayAffinity(hwnd, IsCapture ? WDA_NONE : WDA_EXCLUDEFROMCAPTURE);
			AppliedCapture = IsCapture;
		}
		return Open;
	}

	void Window::BorderlessFullscreen()
	{
		RECT screen;
		GetWindowRect(GetDesktopWindow(), &screen);
		Size = vec2(screen.right, screen.bottom);

		const HWND hwnd = reinterpret_cast<HWND>(Window::Handle);
		SetWindowPos(hwnd, nullptr, Position.x, Position.y, Size.x, Size.y, SWP_NOMOVE | SWP_NOZORDER);
		SetStyle(WS_POPUP);
	}

	int Window::GetStyle()
	{
		const HWND hwnd = reinterpret_cast<HWND>(Window::Handle);
		return GetWindowLongPtr(hwnd, GWL_STYLE);
	}

	void Window::SetStyle(int value)
	{
		const HWND hwnd = reinterpret_cast<HWND>(Window::Handle);
		SetWindowLongPtr(hwnd, GWL_STYLE, value);
	}

	bool Window::IsCursorVisible()
	{
		CURSORINFO cursor = { 0 };
		cursor.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&cursor);
		return cursor.flags & CURSOR_SHOWING;
	}
}

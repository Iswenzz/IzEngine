#include "ImGUI/UI.hpp"
#include "Windows/Base.hpp"

#include "Core/Communication/Actions.hpp"
#include "Core/System/Window.hpp"
#include "Renderer/Core/Renderer.hpp"

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

	void Window::SetCapture(bool state)
	{
		IsCapture = state;
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
			break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			Keyboard::Process(msg, wParam);
			break;

		case WM_MOVE:
			Position = vec2(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_SIZE:
			Size = vec2(LOWORD(lParam), HIWORD(lParam));
			Renderer::Resize(Size);
			break;

		case WM_CLOSE:
			Open = false;
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		if (UI::KeyOpen.IsPressed())
			return true;
		if (UI::Open && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Window::Frame()
	{
		const HWND hwnd = reinterpret_cast<HWND>(Handle);

		MSG msg = { 0 };
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		SetWindowDisplayAffinity(hwnd, IsCapture ? WDA_NONE : WDA_EXCLUDEFROMCAPTURE);
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

	bool Window::IsStyle(int value)
	{
		return GetStyle() == value;
	}

	bool Window::HasStyle(int value)
	{
		return GetStyle() & value;
	}

	bool Window::IsCursorVisible()
	{
		CURSORINFO cursor = { 0 };
		cursor.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&cursor);
		return cursor.flags & CURSOR_SHOWING;
	}
}

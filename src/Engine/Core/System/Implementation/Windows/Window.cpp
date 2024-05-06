#include "ImGUI/UI.hpp"
#include "Windows/Base.hpp"

#include "Core/Input/Keyboard.hpp"
#include "Core/System/Window.hpp"
#include "Renderer/Core/Renderer.hpp"

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return OSWindow::Update(hWnd, uMsg, wParam, lParam);
}

namespace IzEngine
{
	void OSWindow::Initialize(const std::string& name)
	{
		HINSTANCE instance = GetModuleHandle(nullptr);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = WndProc;
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
		Size = vec2(screen.right, screen.bottom);

		HWND hwnd = CreateWindowEx(0, wc.lpszClassName, name.c_str(), WS_POPUP, 0, 0, Size.x, Size.y, nullptr, nullptr,
			instance, nullptr);

		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		Handle = hwnd;
		Open = Handle;
	}

	void OSWindow::Swap(void* handle)
	{
		Handle = handle;
		Open = Handle;
	}

	void OSWindow::Shutdown()
	{
		if (Handle)
			DestroyWindow(reinterpret_cast<HWND>(Handle));
	}

	void OSWindow::SetCapture(bool state)
	{
		IsCapture = state;
	}

	int OSWindow::Update(void* handle, int msg, int wParam, int lParam)
	{
		const HWND hwnd = reinterpret_cast<HWND>(handle);
		const auto& UI = UI::Get();

		switch (msg)
		{
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

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			Keyboard::Process(msg, wParam);
			break;
		}
		if (!UI.Active)
			return DefWindowProc(hwnd, msg, wParam, lParam);

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = UI.Open;

		if (UI.Open && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void OSWindow::Frame()
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
}

#define WIN32_LEAN_AND_MEAN
#define UUID_DEFINED

#include <Windows.h>

#undef min
#undef max

#include <cef_app.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CefMainArgs args(hInstance);
	return CefExecuteProcess(args, nullptr, nullptr);
}

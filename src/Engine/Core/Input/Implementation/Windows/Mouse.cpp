#include "Windows/Base.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Input/Mouse.hpp"

namespace IzEngine
{
	void Mouse::Register(void* window)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = reinterpret_cast<HWND>(window);

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			Log::WriteLine(Channel::Error, "Failed to register raw input device.");
	}

	void Mouse::Process(uint64_t msg, uint64_t state)
	{
		if (msg == WM_INPUT)
		{
			HRAWINPUT hRaw = reinterpret_cast<HRAWINPUT>(state);
			RAWINPUT raw = { 0 };
			UINT size = 0;

			if (GetRawInputData(hRaw, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
				return;
			if (size > sizeof(RAWINPUT))
				return;
			if (GetRawInputData(hRaw, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER)) != size)
				return;
			if (raw.header.dwType != RIM_TYPEMOUSE)
				return;

			DeltaX = raw.data.mouse.lLastX;
			DeltaY = raw.data.mouse.lLastY;

			X += DeltaX;
			Y += DeltaY;

			if (raw.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_DOWN)
				Input::Inputs[Button_Left].State = INPUT_DOWN;
			if (raw.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_1_UP)
				Input::Inputs[Button_Left].State = INPUT_UP;
			if (raw.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_DOWN)
				Input::Inputs[Button_Right].State = INPUT_DOWN;
			if (raw.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_2_UP)
				Input::Inputs[Button_Right].State = INPUT_UP;
			if (raw.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_DOWN)
				Input::Inputs[Button_Middle].State = INPUT_DOWN;
			if (raw.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_3_UP)
				Input::Inputs[Button_Middle].State = INPUT_UP;
		}
	}
}

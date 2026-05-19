#include "Base.hpp"

#include "Engine/Core/Input/Input.hpp"
#include "Engine/Core/Input/Keyboard.hpp"
#include "Engine/Core/System/Window.hpp"

namespace IzEngine
{
	void Keyboard::Register()
	{
		IZ_ASSERT(Window::Handle, "Window is not initialized.");

		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x06;
		rid.dwFlags = 0;
		rid.hwndTarget = reinterpret_cast<HWND>(Window::Handle);

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			Log::WriteLine(Channel::Error, "Failed to register raw input device.");
	}

	void Keyboard::Process(uint64_t msg, uint64_t state)
	{
		if (msg == WM_CHAR)
		{
			Char = static_cast<int>(state);
			return;
		}
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
			if (raw.header.dwType != RIM_TYPEKEYBOARD)
				return;

			USHORT vk = raw.data.keyboard.VKey;
			USHORT scancode = raw.data.keyboard.MakeCode;
			USHORT flags = raw.data.keyboard.Flags;

			if (vk == VK_SHIFT)
				vk = (MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX) == VK_RSHIFT) ? VK_RSHIFT : VK_SHIFT;
			else if (vk == VK_CONTROL)
				vk = (flags & RI_KEY_E0) ? VK_RCONTROL : VK_CONTROL;
			else if (vk == VK_MENU)
				vk = (flags & RI_KEY_E0) ? VK_RMENU : VK_MENU;

			InputEnum index = Input::MapKey(vk);
			if (index)
				Input::Inputs[index].State = (flags & RI_KEY_BREAK) ? INPUT_UP : INPUT_DOWN;
		}
	}
}

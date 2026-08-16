#include "Base.hpp"

#include "Engine/Core/Input/Input.hpp"

namespace IzEngine
{
	void Input::Initialize()
	{
		Inputs = {};
		KeyboardOSToID.fill(Input_None);
		MouseOSToID.clear();

		const auto add = [](InputEnum id, int os, const char* name) { Inputs[id] = InputInfo(id, os, name); };

		add(Key_Tab, VK_TAB, "Tab");
		add(Key_LeftArrow, VK_LEFT, "Left Arrow");
		add(Key_RightArrow, VK_RIGHT, "Right Arrow");
		add(Key_UpArrow, VK_UP, "Up Arrow");
		add(Key_DownArrow, VK_DOWN, "Down Arrow");
		add(Key_PageUp, VK_PRIOR, "Page Up");
		add(Key_PageDown, VK_NEXT, "Page Down");
		add(Key_Home, VK_HOME, "Home");
		add(Key_End, VK_END, "End");
		add(Key_Insert, VK_INSERT, "Insert");
		add(Key_Delete, VK_DELETE, "Delete");
		add(Key_Backspace, VK_BACK, "Backspace");
		add(Key_Space, VK_SPACE, "Space");
		add(Key_Enter, VK_RETURN, "Enter");
		add(Key_Escape, VK_ESCAPE, "Escape");
		add(Key_Apostrophe, VK_OEM_7, "Apostrophe");
		add(Key_Comma, VK_OEM_COMMA, "Comma");
		add(Key_Minus, VK_OEM_MINUS, "Minus");
		add(Key_Period, VK_OEM_PERIOD, "Period");
		add(Key_Slash, VK_OEM_2, "Slash");
		add(Key_Semicolon, VK_OEM_1, "Semicolon");
		add(Key_Equal, VK_OEM_PLUS, "Equal");
		add(Key_LeftBracket, VK_OEM_4, "Left Bracket");
		add(Key_Backslash, VK_OEM_5, "Backslash");
		add(Key_RightBracket, VK_OEM_6, "Right Bracket");
		add(Key_GraveAccent, VK_OEM_3, "Grave Accent");
		add(Key_CapsLock, VK_CAPITAL, "Caps Lock");
		add(Key_ScrollLock, VK_SCROLL, "Scroll Lock");
		add(Key_NumLock, VK_NUMLOCK, "Num Lock");
		add(Key_PrintScreen, VK_SNAPSHOT, "Print Screen");
		add(Key_Pause, VK_PAUSE, "Pause");
		add(Key_Keypad0, VK_NUMPAD0, "Keypad 0");
		add(Key_Keypad1, VK_NUMPAD1, "Keypad 1");
		add(Key_Keypad2, VK_NUMPAD2, "Keypad 2");
		add(Key_Keypad3, VK_NUMPAD3, "Keypad 3");
		add(Key_Keypad4, VK_NUMPAD4, "Keypad 4");
		add(Key_Keypad5, VK_NUMPAD5, "Keypad 5");
		add(Key_Keypad6, VK_NUMPAD6, "Keypad 6");
		add(Key_Keypad7, VK_NUMPAD7, "Keypad 7");
		add(Key_Keypad8, VK_NUMPAD8, "Keypad 8");
		add(Key_Keypad9, VK_NUMPAD9, "Keypad 9");
		add(Key_KeypadDecimal, VK_DECIMAL, "Keypad Decimal");
		add(Key_KeypadDivide, VK_DIVIDE, "Keypad Divide");
		add(Key_KeypadMultiply, VK_MULTIPLY, "Keypad Multiply");
		add(Key_KeypadSubtract, VK_SUBTRACT, "Keypad Subtract");
		add(Key_KeypadAdd, VK_ADD, "Keypad Add");
		add(Key_Shift, VK_SHIFT, "Shift");
		add(Key_Ctrl, VK_CONTROL, "Ctrl");
		add(Key_Alt, VK_MENU, "Alt");
		add(Key_Super, VK_LWIN, "Windows");
		add(Key_RightShift, VK_RSHIFT, "Right Shift");
		add(Key_RightCtrl, VK_RCONTROL, "Right Ctrl");
		add(Key_RightAlt, VK_RMENU, "Right Alt");
		add(Key_RightSuper, VK_RWIN, "Right Windows");
		add(Key_Menu, VK_APPS, "Menu");
		add(Key_0, '0', "0");
		add(Key_1, '1', "1");
		add(Key_2, '2', "2");
		add(Key_3, '3', "3");
		add(Key_4, '4', "4");
		add(Key_5, '5', "5");
		add(Key_6, '6', "6");
		add(Key_7, '7', "7");
		add(Key_8, '8', "8");
		add(Key_9, '9', "9");
		add(Key_A, 'A', "A");
		add(Key_B, 'B', "B");
		add(Key_C, 'C', "C");
		add(Key_D, 'D', "D");
		add(Key_E, 'E', "E");
		add(Key_F, 'F', "F");
		add(Key_G, 'G', "G");
		add(Key_H, 'H', "H");
		add(Key_I, 'I', "I");
		add(Key_J, 'J', "J");
		add(Key_K, 'K', "K");
		add(Key_L, 'L', "L");
		add(Key_M, 'M', "M");
		add(Key_N, 'N', "N");
		add(Key_O, 'O', "O");
		add(Key_P, 'P', "P");
		add(Key_Q, 'Q', "Q");
		add(Key_R, 'R', "R");
		add(Key_S, 'S', "S");
		add(Key_T, 'T', "T");
		add(Key_U, 'U', "U");
		add(Key_V, 'V', "V");
		add(Key_W, 'W', "W");
		add(Key_X, 'X', "X");
		add(Key_Y, 'Y', "Y");
		add(Key_Z, 'Z', "Z");
		add(Key_F1, VK_F1, "F1");
		add(Key_F2, VK_F2, "F2");
		add(Key_F3, VK_F3, "F3");
		add(Key_F4, VK_F4, "F4");
		add(Key_F5, VK_F5, "F5");
		add(Key_F6, VK_F6, "F6");
		add(Key_F7, VK_F7, "F7");
		add(Key_F8, VK_F8, "F8");
		add(Key_F9, VK_F9, "F9");
		add(Key_F10, VK_F10, "F10");
		add(Key_F11, VK_F11, "F11");
		add(Key_F12, VK_F12, "F12");
		add(Button_Left, RI_MOUSE_BUTTON_1_DOWN, "Left Click");
		add(Button_Right, RI_MOUSE_BUTTON_2_DOWN, "Right Click");
		add(Button_Middle, RI_MOUSE_BUTTON_3_DOWN, "Middle Click");

		for (const auto& input : Inputs)
		{
			if (!input.IsRegistered())
				continue;

			if (input.ID >= Button_Left)
				MouseOSToID[input.OS] = input.ID;
			else if (input.OS >= 0 && input.OS < static_cast<int>(KeyboardOSToID.size()))
				KeyboardOSToID[input.OS] = input.ID;
		}
	}
}

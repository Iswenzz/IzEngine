#include "Core/Input/Input.hpp"

namespace IzEngine
{
	void Input::Initialize()
	{
		Inputs = {
			{ Key_Tab, InputInfo(Key_Tab, VK_TAB, "Tab") },
			{ Key_LeftArrow, InputInfo(Key_LeftArrow, VK_LEFT, "Left Arrow") },
			{ Key_RightArrow, InputInfo(Key_RightArrow, VK_RIGHT, "Right Arrow") },
			{ Key_UpArrow, InputInfo(Key_UpArrow, VK_UP, "Up Arrow") },
			{ Key_DownArrow, InputInfo(Key_DownArrow, VK_DOWN, "Down Arrow") },
			{ Key_PageUp, InputInfo(Key_PageUp, VK_PRIOR, "Page Up") },
			{ Key_PageDown, InputInfo(Key_PageDown, VK_NEXT, "Page Down") },
			{ Key_Home, InputInfo(Key_Home, VK_HOME, "Home") },
			{ Key_End, InputInfo(Key_End, VK_END, "End") },
			{ Key_Insert, InputInfo(Key_Insert, VK_INSERT, "Insert") },
			{ Key_Delete, InputInfo(Key_Delete, VK_DELETE, "Delete") },
			{ Key_Backspace, InputInfo(Key_Backspace, VK_BACK, "Backspace") },
			{ Key_Space, InputInfo(Key_Space, VK_SPACE, "Space") },
			{ Key_Enter, InputInfo(Key_Enter, VK_RETURN, "Enter") },
			{ Key_Escape, InputInfo(Key_Escape, VK_ESCAPE, "Escape") },
			{ Key_Apostrophe, InputInfo(Key_Apostrophe, VK_OEM_7, "Apostrophe") },
			{ Key_Comma, InputInfo(Key_Comma, VK_OEM_COMMA, "Comma") },
			{ Key_Minus, InputInfo(Key_Minus, VK_OEM_MINUS, "Minus") },
			{ Key_Period, InputInfo(Key_Period, VK_OEM_PERIOD, "Period") },
			{ Key_Slash, InputInfo(Key_Slash, VK_OEM_2, "Slash") },
			{ Key_Semicolon, InputInfo(Key_Semicolon, VK_OEM_1, "Semicolon") },
			{ Key_Equal, InputInfo(Key_Equal, VK_OEM_PLUS, "Equal") },
			{ Key_LeftBracket, InputInfo(Key_LeftBracket, VK_OEM_4, "Left Bracket") },
			{ Key_Backslash, InputInfo(Key_Backslash, VK_OEM_5, "Backslash") },
			{ Key_RightBracket, InputInfo(Key_RightBracket, VK_OEM_6, "Right Bracket") },
			{ Key_GraveAccent, InputInfo(Key_GraveAccent, VK_OEM_3, "Grave Accent") },
			{ Key_CapsLock, InputInfo(Key_CapsLock, VK_CAPITAL, "Caps Lock") },
			{ Key_ScrollLock, InputInfo(Key_ScrollLock, VK_SCROLL, "Scroll Lock") },
			{ Key_NumLock, InputInfo(Key_NumLock, VK_NUMLOCK, "Num Lock") },
			{ Key_PrintScreen, InputInfo(Key_PrintScreen, VK_SNAPSHOT, "Print Screen") },
			{ Key_Pause, InputInfo(Key_Pause, VK_PAUSE, "Pause") },
			{ Key_Keypad0, InputInfo(Key_Keypad0, VK_NUMPAD0, "Keypad 0") },
			{ Key_Keypad1, InputInfo(Key_Keypad1, VK_NUMPAD1, "Keypad 1") },
			{ Key_Keypad2, InputInfo(Key_Keypad2, VK_NUMPAD2, "Keypad 2") },
			{ Key_Keypad3, InputInfo(Key_Keypad3, VK_NUMPAD3, "Keypad 3") },
			{ Key_Keypad4, InputInfo(Key_Keypad4, VK_NUMPAD4, "Keypad 4") },
			{ Key_Keypad5, InputInfo(Key_Keypad5, VK_NUMPAD5, "Keypad 5") },
			{ Key_Keypad6, InputInfo(Key_Keypad6, VK_NUMPAD6, "Keypad 6") },
			{ Key_Keypad7, InputInfo(Key_Keypad7, VK_NUMPAD7, "Keypad 7") },
			{ Key_Keypad8, InputInfo(Key_Keypad8, VK_NUMPAD8, "Keypad 8") },
			{ Key_Keypad9, InputInfo(Key_Keypad9, VK_NUMPAD9, "Keypad 9") },
			{ Key_KeypadDecimal, InputInfo(Key_KeypadDecimal, VK_DECIMAL, "Keypad Decimal") },
			{ Key_KeypadDivide, InputInfo(Key_KeypadDivide, VK_DIVIDE, "Keypad Divide") },
			{ Key_KeypadMultiply, InputInfo(Key_KeypadMultiply, VK_MULTIPLY, "Keypad Multiply") },
			{ Key_KeypadSubtract, InputInfo(Key_KeypadSubtract, VK_SUBTRACT, "Keypad Subtract") },
			{ Key_KeypadAdd, InputInfo(Key_KeypadAdd, VK_ADD, "Keypad Add") },
			{ Key_LeftShift, InputInfo(Key_LeftShift, VK_LSHIFT, "Left Shift") },
			{ Key_LeftCtrl, InputInfo(Key_LeftCtrl, VK_LCONTROL, "Left Ctrl") },
			{ Key_LeftAlt, InputInfo(Key_LeftAlt, VK_LMENU, "Left Alt") },
			{ Key_LeftSuper, InputInfo(Key_LeftSuper, VK_LWIN, "Left Super") },
			{ Key_RightShift, InputInfo(Key_RightShift, VK_RSHIFT, "Right Shift") },
			{ Key_RightCtrl, InputInfo(Key_RightCtrl, VK_RCONTROL, "Right Ctrl") },
			{ Key_RightAlt, InputInfo(Key_RightAlt, VK_RMENU, "Right Alt") },
			{ Key_RightSuper, InputInfo(Key_RightSuper, VK_RWIN, "Right Super") },
			{ Key_Menu, InputInfo(Key_Menu, VK_APPS, "Menu") },
			{ Key_0, InputInfo(Key_0, '0', "0") },
			{ Key_1, InputInfo(Key_1, '1', "1") },
			{ Key_2, InputInfo(Key_2, '2', "2") },
			{ Key_3, InputInfo(Key_3, '3', "3") },
			{ Key_4, InputInfo(Key_4, '4', "4") },
			{ Key_5, InputInfo(Key_5, '5', "5") },
			{ Key_6, InputInfo(Key_6, '6', "6") },
			{ Key_7, InputInfo(Key_7, '7', "7") },
			{ Key_8, InputInfo(Key_8, '8', "8") },
			{ Key_9, InputInfo(Key_9, '9', "9") },
			{ Key_A, InputInfo(Key_A, 'A', "A") },
			{ Key_B, InputInfo(Key_B, 'B', "B") },
			{ Key_C, InputInfo(Key_C, 'C', "C") },
			{ Key_D, InputInfo(Key_D, 'D', "D") },
			{ Key_E, InputInfo(Key_E, 'E', "E") },
			{ Key_F, InputInfo(Key_F, 'F', "F") },
			{ Key_G, InputInfo(Key_G, 'G', "G") },
			{ Key_H, InputInfo(Key_H, 'H', "H") },
			{ Key_I, InputInfo(Key_I, 'I', "I") },
			{ Key_J, InputInfo(Key_J, 'J', "J") },
			{ Key_K, InputInfo(Key_K, 'K', "K") },
			{ Key_L, InputInfo(Key_L, 'L', "L") },
			{ Key_M, InputInfo(Key_M, 'M', "M") },
			{ Key_N, InputInfo(Key_N, 'N', "N") },
			{ Key_O, InputInfo(Key_O, 'O', "O") },
			{ Key_P, InputInfo(Key_P, 'P', "P") },
			{ Key_Q, InputInfo(Key_Q, 'Q', "Q") },
			{ Key_R, InputInfo(Key_R, 'R', "R") },
			{ Key_S, InputInfo(Key_S, 'S', "S") },
			{ Key_T, InputInfo(Key_T, 'T', "T") },
			{ Key_U, InputInfo(Key_U, 'U', "U") },
			{ Key_V, InputInfo(Key_V, 'V', "V") },
			{ Key_W, InputInfo(Key_W, 'W', "W") },
			{ Key_X, InputInfo(Key_X, 'X', "X") },
			{ Key_Y, InputInfo(Key_Y, 'Y', "Y") },
			{ Key_Z, InputInfo(Key_Z, 'Z', "Z") },
			{ Key_F1, InputInfo(Key_F1, VK_F1, "F1") },
			{ Key_F2, InputInfo(Key_F2, VK_F2, "F2") },
			{ Key_F3, InputInfo(Key_F3, VK_F3, "F3") },
			{ Key_F4, InputInfo(Key_F4, VK_F4, "F4") },
			{ Key_F5, InputInfo(Key_F5, VK_F5, "F5") },
			{ Key_F6, InputInfo(Key_F6, VK_F6, "F6") },
			{ Key_F7, InputInfo(Key_F7, VK_F7, "F7") },
			{ Key_F8, InputInfo(Key_F8, VK_F8, "F8") },
			{ Key_F9, InputInfo(Key_F9, VK_F9, "F9") },
			{ Key_F10, InputInfo(Key_F10, VK_F10, "F10") },
			{ Key_F11, InputInfo(Key_F11, VK_F11, "F11") },
			{ Key_F12, InputInfo(Key_F12, VK_F12, "F12") },
			{ Button_Left, InputInfo(Button_Left, RI_MOUSE_BUTTON_1_DOWN, "Left Click") },
			{ Button_Right, InputInfo(Button_Right, RI_MOUSE_BUTTON_2_DOWN, "Right Click") },
			{ Button_Middle, InputInfo(Button_Middle, RI_MOUSE_BUTTON_3_DOWN, "Middle Click") },
		};
		for (const auto& [_, input] : Inputs)
			OSToID[input.OS] = input.ID;
	}

	void Input::Reset()
	{
		for (auto& [_, input] : Inputs)
		{
			input.PrevState = input.State;
			if (input.State == INPUT_UP)
				input.State = INPUT_NONE;
		}
	}

	bool Input::IsUp(InputEnum input)
	{
		return Inputs[input].State == INPUT_UP;
	}

	bool Input::IsDown(InputEnum input)
	{
		return Inputs[input].State == INPUT_DOWN;
	}

	bool Input::IsPressed(InputEnum input)
	{
		return Inputs[input].PrevState == INPUT_NONE && IsDown(input);
	}

	InputEnum Input::Map(int input)
	{
		return OSToID[input];
	}

	const char* Input::GetName(InputEnum input)
	{
		return Inputs[input].Name.c_str();
	}
}

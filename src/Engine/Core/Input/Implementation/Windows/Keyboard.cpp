#include "Windows/Base.hpp"

#include "Core/Input/Keyboard.hpp"

namespace IW3SR::Engine
{
	Keyboard::Keyboard(int key)
	{
		Key = key;
	}

	bool Keyboard::IsUp()
	{
		return Keys[Key].State == WM_KEYUP;
	}

	bool Keyboard::IsDown()
	{
		return Keys[Key].State == WM_KEYDOWN;
	}

	bool Keyboard::IsPressed()
	{
		return Keys[Key].PrevState == WM_NULL && IsDown();
	}

	bool Keyboard::IsUp(int key)
	{
		return Keyboard(key).IsUp();
	}

	bool Keyboard::IsDown(int key)
	{
		return Keyboard(key).IsDown();
	}

	bool Keyboard::IsPressed(int key)
	{
		return Keyboard(key).IsPressed();
	}

	void Keyboard::Process(int msg, int key)
	{
		switch (msg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			Keys[Map(key)].State = WM_KEYDOWN;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keys[Map(key)].State = WM_KEYUP;
			break;
		}
	}

	void Keyboard::Reset()
	{
		for (auto& [key, info] : Keys)
		{
			info.PrevState = info.State;
			if (info.State == WM_KEYUP)
				info.State = WM_NULL;
		}
	}

	int Keyboard::Map(int key)
	{
		switch (key)
		{
		case VK_TAB:
			return Key_Tab;
		case VK_LEFT:
			return Key_LeftArrow;
		case VK_RIGHT:
			return Key_RightArrow;
		case VK_UP:
			return Key_UpArrow;
		case VK_DOWN:
			return Key_DownArrow;
		case VK_PRIOR:
			return Key_PageUp;
		case VK_NEXT:
			return Key_PageDown;
		case VK_HOME:
			return Key_Home;
		case VK_END:
			return Key_End;
		case VK_INSERT:
			return Key_Insert;
		case VK_DELETE:
			return Key_Delete;
		case VK_BACK:
			return Key_Backspace;
		case VK_SPACE:
			return Key_Space;
		case VK_RETURN:
			return Key_Enter;
		case VK_ESCAPE:
			return Key_Escape;
		case VK_OEM_7:
			return Key_Apostrophe;
		case VK_OEM_COMMA:
			return Key_Comma;
		case VK_OEM_MINUS:
			return Key_Minus;
		case VK_OEM_PERIOD:
			return Key_Period;
		case VK_OEM_2:
			return Key_Slash;
		case VK_OEM_1:
			return Key_Semicolon;
		case VK_OEM_PLUS:
			return Key_Equal;
		case VK_OEM_4:
			return Key_LeftBracket;
		case VK_OEM_5:
			return Key_Backslash;
		case VK_OEM_6:
			return Key_RightBracket;
		case VK_OEM_3:
			return Key_GraveAccent;
		case VK_CAPITAL:
			return Key_CapsLock;
		case VK_SCROLL:
			return Key_ScrollLock;
		case VK_NUMLOCK:
			return Key_NumLock;
		case VK_SNAPSHOT:
			return Key_PrintScreen;
		case VK_PAUSE:
			return Key_Pause;
		case VK_NUMPAD0:
			return Key_Keypad0;
		case VK_NUMPAD1:
			return Key_Keypad1;
		case VK_NUMPAD2:
			return Key_Keypad2;
		case VK_NUMPAD3:
			return Key_Keypad3;
		case VK_NUMPAD4:
			return Key_Keypad4;
		case VK_NUMPAD5:
			return Key_Keypad5;
		case VK_NUMPAD6:
			return Key_Keypad6;
		case VK_NUMPAD7:
			return Key_Keypad7;
		case VK_NUMPAD8:
			return Key_Keypad8;
		case VK_NUMPAD9:
			return Key_Keypad9;
		case VK_DECIMAL:
			return Key_KeypadDecimal;
		case VK_DIVIDE:
			return Key_KeypadDivide;
		case VK_MULTIPLY:
			return Key_KeypadMultiply;
		case VK_SUBTRACT:
			return Key_KeypadSubtract;
		case VK_ADD:
			return Key_KeypadAdd;
		case VK_LSHIFT:
			return Key_LeftShift;
		case VK_LCONTROL:
			return Key_LeftCtrl;
		case VK_LMENU:
			return Key_LeftAlt;
		case VK_LWIN:
			return Key_LeftSuper;
		case VK_RSHIFT:
			return Key_RightShift;
		case VK_RCONTROL:
			return Key_RightCtrl;
		case VK_RMENU:
			return Key_RightAlt;
		case VK_RWIN:
			return Key_RightSuper;
		case VK_APPS:
			return Key_Menu;
		case '0':
			return Key_0;
		case '1':
			return Key_1;
		case '2':
			return Key_2;
		case '3':
			return Key_3;
		case '4':
			return Key_4;
		case '5':
			return Key_5;
		case '6':
			return Key_6;
		case '7':
			return Key_7;
		case '8':
			return Key_8;
		case '9':
			return Key_9;
		case 'A':
			return Key_A;
		case 'B':
			return Key_B;
		case 'C':
			return Key_C;
		case 'D':
			return Key_D;
		case 'E':
			return Key_E;
		case 'F':
			return Key_F;
		case 'G':
			return Key_G;
		case 'H':
			return Key_H;
		case 'I':
			return Key_I;
		case 'J':
			return Key_J;
		case 'K':
			return Key_K;
		case 'L':
			return Key_L;
		case 'M':
			return Key_M;
		case 'N':
			return Key_N;
		case 'O':
			return Key_O;
		case 'P':
			return Key_P;
		case 'Q':
			return Key_Q;
		case 'R':
			return Key_R;
		case 'S':
			return Key_S;
		case 'T':
			return Key_T;
		case 'U':
			return Key_U;
		case 'V':
			return Key_V;
		case 'W':
			return Key_W;
		case 'X':
			return Key_X;
		case 'Y':
			return Key_Y;
		case 'Z':
			return Key_Z;
		case VK_F1:
			return Key_F1;
		case VK_F2:
			return Key_F2;
		case VK_F3:
			return Key_F3;
		case VK_F4:
			return Key_F4;
		case VK_F5:
			return Key_F5;
		case VK_F6:
			return Key_F6;
		case VK_F7:
			return Key_F7;
		case VK_F8:
			return Key_F8;
		case VK_F9:
			return Key_F9;
		case VK_F10:
			return Key_F10;
		case VK_F11:
			return Key_F11;
		case VK_F12:
			return Key_F12;
		}
		return Key_None;
	}

	const char* Keyboard::GetName(int key)
	{
		switch (key)
		{
		case Key_Tab:
			return "Tab";
		case Key_LeftArrow:
			return "Left Arrow";
		case Key_RightArrow:
			return "Right Arrow";
		case Key_UpArrow:
			return "Up Arrow";
		case Key_DownArrow:
			return "Down Arrow";
		case Key_PageUp:
			return "Page Up";
		case Key_PageDown:
			return "Page Down";
		case Key_Home:
			return "Home";
		case Key_End:
			return "End";
		case Key_Insert:
			return "Insert";
		case Key_Delete:
			return "Delete";
		case Key_Backspace:
			return "Backspace";
		case Key_Space:
			return "Space";
		case Key_Enter:
			return "Enter";
		case Key_Escape:
			return "Escape";
		case Key_Apostrophe:
			return "Apostrophe";
		case Key_Comma:
			return "Comma";
		case Key_Minus:
			return "Minus";
		case Key_Period:
			return "Period";
		case Key_Slash:
			return "Slash";
		case Key_Semicolon:
			return "Semicolon";
		case Key_Equal:
			return "Equal";
		case Key_LeftBracket:
			return "Left Bracket";
		case Key_Backslash:
			return "Backslash";
		case Key_RightBracket:
			return "Right Bracket";
		case Key_GraveAccent:
			return "Grave Accent";
		case Key_CapsLock:
			return "Caps Lock";
		case Key_ScrollLock:
			return "Scroll Lock";
		case Key_NumLock:
			return "Num Lock";
		case Key_PrintScreen:
			return "Print Screen";
		case Key_Pause:
			return "Pause";
		case Key_Keypad0:
			return "Keypad 0";
		case Key_Keypad1:
			return "Keypad 1";
		case Key_Keypad2:
			return "Keypad 2";
		case Key_Keypad3:
			return "Keypad 3";
		case Key_Keypad4:
			return "Keypad 4";
		case Key_Keypad5:
			return "Keypad 5";
		case Key_Keypad6:
			return "Keypad 6";
		case Key_Keypad7:
			return "Keypad 7";
		case Key_Keypad8:
			return "Keypad 8";
		case Key_Keypad9:
			return "Keypad 9";
		case Key_KeypadDecimal:
			return "Keypad Decimal";
		case Key_KeypadDivide:
			return "Keypad Divide";
		case Key_KeypadMultiply:
			return "Keypad Multiply";
		case Key_KeypadSubtract:
			return "Keypad Subtract";
		case Key_KeypadAdd:
			return "Keypad Add";
		case Key_LeftShift:
			return "Shift";
		case Key_LeftCtrl:
			return "Control";
		case Key_LeftAlt:
			return "Alt";
		case Key_LeftSuper:
			return "Super";
		case Key_RightShift:
			return "Right Shift";
		case Key_RightCtrl:
			return "Right Control";
		case Key_RightAlt:
			return "Right Alt";
		case Key_RightSuper:
			return "Right Super";
		case Key_Menu:
			return "Menu";
		case Key_0:
			return "0";
		case Key_1:
			return "1";
		case Key_2:
			return "2";
		case Key_3:
			return "3";
		case Key_4:
			return "4";
		case Key_5:
			return "5";
		case Key_6:
			return "6";
		case Key_7:
			return "7";
		case Key_8:
			return "8";
		case Key_9:
			return "9";
		case Key_A:
			return "A";
		case Key_B:
			return "B";
		case Key_C:
			return "C";
		case Key_D:
			return "D";
		case Key_E:
			return "E";
		case Key_F:
			return "F";
		case Key_G:
			return "G";
		case Key_H:
			return "H";
		case Key_I:
			return "I";
		case Key_J:
			return "J";
		case Key_K:
			return "K";
		case Key_L:
			return "L";
		case Key_M:
			return "M";
		case Key_N:
			return "N";
		case Key_O:
			return "O";
		case Key_P:
			return "P";
		case Key_Q:
			return "Q";
		case Key_R:
			return "R";
		case Key_S:
			return "S";
		case Key_T:
			return "T";
		case Key_U:
			return "U";
		case Key_V:
			return "V";
		case Key_W:
			return "W";
		case Key_X:
			return "X";
		case Key_Y:
			return "Y";
		case Key_Z:
			return "Z";
		case Key_F1:
			return "F1";
		case Key_F2:
			return "F2";
		case Key_F3:
			return "F3";
		case Key_F4:
			return "F4";
		case Key_F5:
			return "F5";
		case Key_F6:
			return "F6";
		case Key_F7:
			return "F7";
		case Key_F8:
			return "F8";
		case Key_F9:
			return "F9";
		case Key_F10:
			return "F10";
		case Key_F11:
			return "F11";
		case Key_F12:
			return "F12";
		}
		return "None";
	}
}

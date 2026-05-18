#pragma once
#include "Engine/Core/Base.hpp"

#define INPUT_NONE 0
#define INPUT_DOWN 1
#define INPUT_UP 2

namespace IzEngine
{
	enum InputEnum : int
	{
		Input_None,
		Key_Tab,
		Key_LeftArrow,
		Key_RightArrow,
		Key_UpArrow,
		Key_DownArrow,
		Key_PageUp,
		Key_PageDown,
		Key_Home,
		Key_End,
		Key_Insert,
		Key_Delete,
		Key_Backspace,
		Key_Space,
		Key_Enter,
		Key_Escape,
		Key_Ctrl,
		Key_Shift,
		Key_Alt,
		Key_Super,
		Key_RightCtrl,
		Key_RightShift,
		Key_RightAlt,
		Key_RightSuper,
		Key_Menu,
		Key_0,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Key_A,
		Key_B,
		Key_C,
		Key_D,
		Key_E,
		Key_F,
		Key_G,
		Key_H,
		Key_I,
		Key_J,
		Key_K,
		Key_L,
		Key_M,
		Key_N,
		Key_O,
		Key_P,
		Key_Q,
		Key_R,
		Key_S,
		Key_T,
		Key_U,
		Key_V,
		Key_W,
		Key_X,
		Key_Y,
		Key_Z,
		Key_F1,
		Key_F2,
		Key_F3,
		Key_F4,
		Key_F5,
		Key_F6,
		Key_F7,
		Key_F8,
		Key_F9,
		Key_F10,
		Key_F11,
		Key_F12,
		Key_Apostrophe,
		Key_Comma,
		Key_Minus,
		Key_Period,
		Key_Slash,
		Key_Semicolon,
		Key_Equal,
		Key_LeftBracket,
		Key_Backslash,
		Key_RightBracket,
		Key_GraveAccent,
		Key_CapsLock,
		Key_ScrollLock,
		Key_NumLock,
		Key_PrintScreen,
		Key_Pause,
		Key_Keypad0,
		Key_Keypad1,
		Key_Keypad2,
		Key_Keypad3,
		Key_Keypad4,
		Key_Keypad5,
		Key_Keypad6,
		Key_Keypad7,
		Key_Keypad8,
		Key_Keypad9,
		Key_KeypadDecimal,
		Key_KeypadDivide,
		Key_KeypadMultiply,
		Key_KeypadSubtract,
		Key_KeypadAdd,
		Key_KeypadEnter,
		Key_KeypadEqual,
		Key_GamepadStart,
		Key_GamepadBack,
		Key_GamepadFaceLeft,
		Key_GamepadFaceRight,
		Key_GamepadFaceUp,
		Key_GamepadFaceDown,
		Key_GamepadDpadLeft,
		Key_GamepadDpadRight,
		Key_GamepadDpadUp,
		Key_GamepadDpadDown,
		Key_GamepadL1,
		Key_GamepadR1,
		Key_GamepadL2,
		Key_GamepadR2,
		Key_GamepadL3,
		Key_GamepadR3,
		Key_GamepadLStickLeft,
		Key_GamepadLStickRight,
		Key_GamepadLStickUp,
		Key_GamepadLStickDown,
		Key_GamepadRStickLeft,
		Key_GamepadRStickRight,
		Key_GamepadRStickUp,
		Key_GamepadRStickDown,
		Key_MouseLeft,
		Key_MouseRight,
		Key_MouseMiddle,
		Key_MouseX1,
		Key_MouseX2,
		Key_MouseWheelX,
		Key_MouseWheelY,
		Key_ModNone,
		Key_ModCtrl,
		Key_ModShift,
		Key_ModAlt,
		Key_ModSuper,
		Key_ModShortcut,
		Button_Left,
		Button_Right,
		Button_Middle,
		Input_Count,
	};

	struct InputInfo
	{
		InputEnum ID = InputEnum::Input_None;
		int OS = 0;
		std::string Name;
		int State = 0;
		int PrevState = 0;
	};

	class API Input
	{
	public:
		static inline std::unordered_map<InputEnum, InputInfo> Inputs;
		static inline std::unordered_map<int, InputEnum> KeyboardOSToID;
		static inline std::unordered_map<int, InputEnum> MouseOSToID;

		static void Initialize();
		static void Reset();

		static bool IsUp(InputEnum input);
		static bool IsDown(InputEnum input);
		static bool IsPressed(InputEnum input);

		static InputEnum MapKey(int input);
		static InputEnum MapMouse(int input);
		static const char* GetName(InputEnum input);
	};
}

#include "Input.hpp"

namespace IzEngine
{
	void Input::Reset()
	{
		for (auto& [_, input] : Inputs)
		{
			input.PrevState = input.State;
			if (input.State == INPUT_UP)
				input.State = INPUT_NONE;
		}
		Mouse::Delta = { 0, 0 };
		Mouse::ScrollDelta = 0;
		Keyboard::Char = 0;
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
		return Inputs[input].PrevState == INPUT_NONE && Inputs[input].State == INPUT_DOWN;
	}

	InputEnum Input::MapKey(int input)
	{
		return KeyboardOSToID[input];
	}

	InputEnum Input::MapMouse(int input)
	{
		return MouseOSToID[input];
	}

	const char* Input::GetName(InputEnum input)
	{
		return Inputs[input].Name.c_str();
	}
}

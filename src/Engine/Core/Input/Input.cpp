#include "Input.hpp"

namespace IzEngine
{
	void Input::Reset()
	{
		for (auto& input : Inputs)
		{
			input.PrevState = input.State;
			if (input.State == INPUT_UP)
				input.State = INPUT_NONE;
		}
		Mouse::Delta = { 0, 0 };
		Mouse::ScrollDelta = 0;
		Keyboard::Char = 0;
	}

	const InputInfo* Input::Find(InputEnum input)
	{
		if (input <= Input_None || input >= Input_Count)
			return nullptr;

		const InputInfo& info = Inputs[input];
		return info.IsRegistered() ? &info : nullptr;
	}

	bool Input::IsUp(InputEnum input)
	{
		const auto info = Find(input);
		return info && info->State == INPUT_UP;
	}

	bool Input::IsDown(InputEnum input)
	{
		const auto info = Find(input);
		return info && info->State == INPUT_DOWN;
	}

	bool Input::IsPressed(InputEnum input)
	{
		const auto info = Find(input);
		return info && info->PrevState == INPUT_NONE && info->State == INPUT_DOWN;
	}

	void Input::SetState(InputEnum input, int state)
	{
		if (!Find(input))
			return;

		Inputs[input].State = state;
	}

	InputEnum Input::MapKey(int input)
	{
		if (input < 0 || input >= static_cast<int>(KeyboardOSToID.size()))
			return Input_None;

		return KeyboardOSToID[input];
	}

	InputEnum Input::MapMouse(int input)
	{
		const auto it = MouseOSToID.find(input);
		return it != MouseOSToID.end() ? it->second : Input_None;
	}

	const char* Input::GetName(InputEnum input)
	{
		const auto info = Find(input);
		return info ? info->Name.c_str() : "None";
	}
}

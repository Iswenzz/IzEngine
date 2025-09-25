#include "Windows/Base.hpp"

#include "Core/Input/Input.hpp"
#include "Core/Input/Keyboard.hpp"

namespace IzEngine
{
	void Keyboard::Register(void* window) { }

	void Keyboard::Process(uint64_t msg, uint64_t input)
	{
		InputEnum index = Input::Map(input);
		if (!index)
			return;

		if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)
			Input::Inputs[index].State = INPUT_DOWN;

		if (msg == WM_KEYUP || msg == WM_SYSKEYUP)
			Input::Inputs[index].State = INPUT_UP;
	}
}

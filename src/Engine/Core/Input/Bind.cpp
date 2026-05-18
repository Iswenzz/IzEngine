#include "Bind.hpp"

namespace IzEngine
{
	Bind::Bind(InputEnum input)
	{
		Input = input;
	}

	bool Bind::IsUp()
	{
		return Input::IsUp(Input);
	}

	bool Bind::IsDown()
	{
		return Input::IsDown(Input);
	}

	bool Bind::IsPressed()
	{
		return Input::IsPressed(Input);
	}
}

#pragma once
#include "Input.hpp"

namespace IzEngine
{
	class API Bind
	{
	public:
		InputEnum Input = InputEnum::Input_None;

		Bind() = default;
		Bind(InputEnum input);
		~Bind() = default;

		bool IsUp();
		bool IsDown();
		bool IsPressed();

		SERIALIZE(Bind, Input)
	};
}

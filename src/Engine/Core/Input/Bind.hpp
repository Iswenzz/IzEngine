#pragma once
#include "Core/Base.hpp"

#include "Core/Input/Input.hpp"

namespace IzEngine
{
	/// <summary>
	/// Bind input class.
	/// </summary>
	class API Bind
	{
	public:
		InputEnum Input = InputEnum::Input_None;

		/// <summary>
		/// Initialize the bind.
		/// </summary>
		Bind() = default;

		/// <summary>
		/// Initialize the bind.
		/// </summary>
		/// <param name="input">The input.</param>
		Bind(InputEnum input);
		~Bind() = default;

		/// <summary>
		/// Is up.
		/// </summary>
		bool IsUp();

		/// <summary>
		/// Is down.
		/// </summary>
		bool IsDown();

		/// <summary>
		/// Is pressed.
		/// </summary>
		bool IsPressed();

		SERIALIZE(Bind, Input)
	};
}

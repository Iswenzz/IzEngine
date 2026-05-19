#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class StackCanary
	{
	public:
		StackCanary();
		~StackCanary() noexcept(false);

	private:
		unsigned char Buffer[50];
	};
}

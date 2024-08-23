#include "StackCanary.hpp"

namespace IzEngine
{
	StackCanary::StackCanary()
	{
		for (int i = 0; i < 50; i++)
			Buffer[i] = 0xCE;
	}

	StackCanary::~StackCanary() noexcept(false)
	{
		for (int i = 0; i < 50; i++)
			if (Buffer[i] != 0xCE)
				throw "Stack corruption";
	}
}

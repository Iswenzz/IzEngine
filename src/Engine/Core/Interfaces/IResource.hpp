#pragma once
#include "IObject.hpp"

namespace IzEngine
{
	class IResource : public IObject
	{
	public:
		virtual ~IResource() = default;
	};
}

#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API UUID
	{
	public:
		uint64_t Value;
		std::string String;

		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const;
		operator const std::string&() const;
	};
}

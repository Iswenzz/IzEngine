#pragma once
#include <cstdint>
#include <string>

namespace IW3SR::Engine
{
	/// <summary>
	/// Unique identifier.
	/// </summary>
	class UUID
	{
	public:
		uint64_t Value;
		std::string String;

		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		/// <summary>
		/// Cast to int.
		/// </summary>
		operator uint64_t() const;

		/// <summary>
		/// Cast to string.
		/// </summary>
		operator const std::string&() const;
	};
}

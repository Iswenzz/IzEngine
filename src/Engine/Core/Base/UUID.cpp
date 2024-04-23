#include "UUID.hpp"
#include <random>

namespace IzEngine
{
	static std::random_device Device;
	static std::mt19937_64 Engine(Device());
	static std::uniform_int_distribution<uint64_t> Distribution;

	UUID::UUID() : Value(Distribution(Engine)), String(std::to_string(Value)) { }
	UUID::UUID(uint64_t uuid) : Value(uuid), String(std::to_string(Value)) { }

	UUID::operator uint64_t() const
	{
		return Value;
	}

	UUID::operator const std::string&() const
	{
		return String;
	}
}

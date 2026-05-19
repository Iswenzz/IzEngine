#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API Memory
	{
	public:
		static void Read(uintptr_t address, void* data, size_t size);
		static void Write(uintptr_t address, const std::string& bytes);
		static void Write(uintptr_t address, const std::vector<uint8_t>& bytes);

		static void NOP(uintptr_t address, int size);
		static void JMP(uintptr_t address, uintptr_t to, int size = 5);
		static void CALL(uintptr_t address, uintptr_t to, int size = 5);

		static uintptr_t LE(uintptr_t value);
		static uintptr_t BE(uintptr_t value);

		static std::string Pattern(const std::string& pattern);

		template <typename T>
		static inline void Set(uintptr_t address, T value)
		{
			Write(address, std::string(reinterpret_cast<char*>(&value), sizeof(T)));
		}

		template <typename T>
		static inline T Get(uintptr_t address)
		{
			T value{};
			Read(address, &value, sizeof(T));
			return value;
		}
	};
}

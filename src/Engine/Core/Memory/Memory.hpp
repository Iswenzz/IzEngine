#pragma once
#include "Engine/Base.hpp"

namespace IzEngine
{
	/// <summary>
	/// Memory class.
	/// </summary>
	class API Memory
	{
	public:
		/// <summary>
		/// Read memory.
		/// </summary>
		/// <param name="address">The target address.</param>
		/// <param name="data">The output buffer.</param>
		/// <param name="size">The size of bytes.</param>
		static void Read(uintptr_t address, void* data, int size);

		/// <summary>
		/// Write memory.
		/// </summary>
		/// <param name="address">The target address.</param>
		/// <param name="bytes">The bytes to write.</param>
		static void Write(uintptr_t address, const std::string& bytes);

		/// <summary>
		/// Set value.
		/// </summary>
		/// <typeparam name="T">The type to set.</typeparam>
		/// <param name="address">The address.</param>
		/// <param name="value">The value.</param>
		template <typename T>
		static inline void Set(uintptr_t address, T value)
		{
			WriteBytes(address, std::string(reinterpret_cast<char*>(&value), sizeof(T)));
		}

		/// <summary>
		/// Get value.
		/// </summary>
		/// <typeparam name="T">The type to get.</typeparam>
		/// <param name="address">The address.</param>
		/// <param name="value">The value.</param>
		template <typename T>
		static inline T Get(uintptr_t address, T value)
		{
			T value;
			Read(address, &value, sizeof(T));
			return value;
		}

		/// <summary>
		/// NOP instruction.
		/// </summary>
		/// <param name="address">The target address.</param>
		/// <param name="size">The bytes size.</param>
		static void NOP(uintptr_t address, int size);

		/// <summary>
		/// JMP instruction.
		/// </summary>
		/// <param name="address">The target address.</param>
		/// <param name="to">The jmp address.</param>
		/// <param name="size">The bytes size.</param>
		static void JMP(uintptr_t address, uintptr_t to, int size = 5);

		/// <summary>
		/// CALL instruction.
		/// </summary>
		/// <param name="address">The target address.</param>
		/// <param name="to">The call address.</param>
		/// <param name="size">The bytes size.</param>
		static void CALL(uintptr_t address, uintptr_t to, int size = 5);

		/// <summary>
		/// Convert to little endian.
		/// </summary>
		/// <param name="value">The value.</param>
		/// <returns></returns>
		static uintptr_t LE(uintptr_t value);

		/// <summary>
		/// Convert to big endian.
		/// </summary>
		/// <param name="value">The value.</param>
		/// <returns></returns>
		static uintptr_t BE(uintptr_t value);

		/// <summary>
		/// Get the bytes from a pattern.
		/// </summary>
		/// <param name="pattern">The pattern.</param>
		/// <returns></returns>
		static std::string Pattern(const std::string& pattern);
	};
}

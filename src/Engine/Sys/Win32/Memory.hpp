#pragma once
#include "Engine/Sys/Win32.hpp"

#include <string>
#include <vector>
#include <bit>

namespace IW3SR::Engine
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
        /// <param name="pattern">The pattern to write.</param>
        static inline void Write(uintptr_t address, const std::string& pattern);

        /// <summary>
        /// Write memory.
        /// </summary>
        /// <param name="address">The target address.</param>
        /// <param name="bytes">The bytes to write.</param>
        static void WriteBytes(uintptr_t address, const std::string& bytes);

        /// <summary>
        /// Assign values from reinterpret cast.
        /// </summary>
        /// <typeparam name="R">The reinterpret cast type.</typeparam>
        /// <typeparam name="T">The first type to set.</typeparam>
        /// <typeparam name="U">The second type to set.</typeparam>
        /// <param name="dst">The destination.</param>
        /// <param name="src">The source.</param>
        template <typename R, typename T, typename U>
        static inline void Assign(T& dst, U& src)
        {
            *reinterpret_cast<R*>(&dst) = *reinterpret_cast<R*>(&src);
        }

        /// <summary>
        /// Assign array from reinterpret cast.
        /// </summary>
        /// <typeparam name="R">The reinterpret cast type.</typeparam>
        /// <typeparam name="T">The first type to set.</typeparam>
        /// <typeparam name="U">The second type to set.</typeparam>
        /// <param name="dst">The destination.</param>
        /// <param name="src">The source.</param>
        template <int size, typename R, typename T, typename U>
        static inline void AssignArray(T& dst, U& src)
        {
            std::copy_n(static_cast<R*>(src), size, dst);
        }

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
        static inline void NOP(uintptr_t address, int size);

        /// <summary>
        /// JMP instruction.
        /// </summary>
        /// <param name="address">The target address.</param>
        /// <param name="to">The jmp address.</param>
        /// <param name="size">The bytes size.</param>
        static inline void JMP(uintptr_t address, uintptr_t to, int size = 5);

        /// <summary>
        /// CALL instruction.
        /// </summary>
        /// <param name="address">The target address.</param>
        /// <param name="to">The call address.</param>
        /// <param name="size">The bytes size.</param>
        static inline void CALL(uintptr_t address, uintptr_t to, int size = 5);

        /// <summary>
        /// Convert to little endian.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns></returns>
        constexpr static inline uintptr_t LE(uintptr_t value);

        /// <summary>
        /// Convert to big endian.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns></returns>
        constexpr static inline uintptr_t BE(uintptr_t value);

        /// <summary>
        /// Hex to bytes.
        /// </summary>
        /// <param name="pattern">The pattern.</param>
        /// <returns></returns>
        constexpr static std::string HexToBytes(const std::string& pattern);
    };
}

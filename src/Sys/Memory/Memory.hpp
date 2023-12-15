#pragma once
#include "Utils/Macros.hpp"

#include <string>
#include <vector>
#include <bit>

namespace IW3SR
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
        /// <param name="size">The size of the bytes.</param>
        static void Write(uintptr_t address, const char* bytes, int size);

        /// <summary>
        /// Memory scan.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="bytes">The bytes to scan.</param>
        /// <param name="size">The bytes size.</param>
        /// <returns></returns>
        static uintptr_t Scan(std::string moduleName, const char* bytes, size_t size);

        /// <summary>
        /// Memory scan all results.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="bytes">The bytes to scan.</param>
        /// <param name="size">The bytes size.</param>
        /// <param name="first">Should only scan until the first result.</param>
        /// <returns></returns>
        static std::vector<uintptr_t> ScanAll(std::string moduleName, const char* bytes, size_t size, bool first);

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
        /// Set bytes.
        /// </summary>
        /// <typeparam name="T">The type to set.</typeparam>
        /// <param name="address">The address.</param>
        /// <param name="value">The value.</param>
        template <typename T>
        static void Set(uintptr_t address, T value)
        {
            int size = sizeof(T);
            std::string bytes(reinterpret_cast<char*>(&value), size);
            Write(address, bytes.c_str(), size);
        }

        /// <summary>
        /// NOP instruction.
        /// </summary>
        /// <param name="address"></param>
        /// <param name="size"></param>
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
    };
}

#pragma once
#include <string>
#include <vector>

namespace IW3SR
{
    /// <summary>
    /// Memory class.
    /// </summary>
    class Memory
    {
    public:
        /// <summary>
        /// Write memory.
        /// </summary>
        /// <param name="address">The target address.</param>
        /// <param name="bytes">The bytes to write.</param>
        /// <param name="size">The size of the bytes.</param>
        static void Write(uintptr_t address, std::string bytes, int size);

        /// <summary>
        /// Memory scan.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="bytes">The bytes to scan.</param>
        /// <param name="size">The bytes size.</param>
        /// <returns></returns>
        static uintptr_t Scan(std::string moduleName, std::string bytes, size_t size);

        /// <summary>
        /// Memory scan all results.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="bytes">The bytes to scan.</param>
        /// <param name="size">The bytes size.</param>
        /// <param name="first">Should only scan until the first result.</param>
        /// <returns></returns>
        static std::vector<uintptr_t> ScanAll(std::string moduleName, std::string bytes, size_t size, bool first);

        /// <summary>
        /// Reinterpret set bytes.
        /// </summary>
        /// <typeparam name="T">The type to set.</typeparam>
        /// <param name="dst">The destination.</param>
        /// <param name="src">The source.</param>
        template <typename R, typename T>
        static void Reinterpret(T dst, T src)
        {
            *reinterpret_cast<R*>(dst) = *reinterpret_cast<R*>(src);
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
            Write(address, bytes, size);
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
        /// <param name="to">The detour address.</param>
        /// <param name="size">The bytes size.</param>
        static void JMP(uintptr_t address, uintptr_t to, int size = 5);

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

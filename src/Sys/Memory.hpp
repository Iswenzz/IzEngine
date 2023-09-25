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

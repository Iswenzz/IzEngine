#pragma once
#include "Engine/Sys/Win32.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Signature class.
	/// </summary>
	class API Signature
	{
	public:
        uintptr_t Address = 0;

        /// <summary>
        /// Create a new signature.
        /// </summary>
        Signature() = default;
        ~Signature() = default;

        /// <summary>
        /// Create a new signature.
        /// </summary>
        /// <param name="address">The address.</param>
        Signature(uintptr_t address);

        /// <summary>
        /// Create a new signature.
        /// </summary>
        /// <param name="base">The base address.</param>
        /// <param name="address">The address.</param>
        Signature(uintptr_t base, uintptr_t address);

        /// <summary>
        /// Create a new signature.
        /// </summary>
        /// <param name="hModule">The module handle.</param>
        Signature(HMODULE hModule);

        /// <summary>
        /// Create a new signature.
        /// </summary>
        /// <param name="function">The function.</param>
        Signature(FARPROC function);

        /// <summary>
        /// Create a new signature.
        /// </summary>
        /// <param name="pattern">The pattern to scan.</param>
        Signature(const std::string& pattern);

        /// <summary>
        /// Create a new signature.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="pattern">The pattern to scan.</param>
        Signature(const std::string& moduleName, const std::string& pattern);

        /// <summary>
        /// Memory scan.
        /// </summary
        /// <param name="pattern">The pattern to scan.</param>
        /// <returns></returns>
        inline uintptr_t Scan(const std::string& pattern);

        /// <summary>
        /// Memory scan.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="pattern">The pattern to scan.</param>
        /// <returns></returns>
        uintptr_t Scan(const std::string& moduleName, const std::string& pattern);

        /// <summary>
        /// Memory scan all results.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="pattern">The pattern to scan.</param>
        /// <param name="first">Should only scan until the first result.</param>
        /// <returns></returns>
        std::vector<uintptr_t> ScanAll(const std::string& moduleName, const std::string& pattern, bool first);

        /// <summary>
        /// Get the address.
        /// </summary>
        inline operator uintptr_t()
        {
            return Address;
        }

        /// <summary>
        /// Get the address.
        /// </summary>
        template <typename T>
        inline operator T*()
        {
            return reinterpret_cast<T*>(Address);
        }

        /// <summary>
        /// Get the address.
        /// </summary>
        template <typename T>
        inline operator T&()
        {
            return *reinterpret_cast<T*>(Address);
        }
	};
}

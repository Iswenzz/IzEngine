#pragma once
#include "Windows/Base.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Signature class.
	/// </summary>
	class Sig
	{
	public:
		Sig() = default;
		~Sig() = default;

        /// <summary>
        /// Memory scan.
        /// </summary
        /// <param name="pattern">The pattern to scan.</param>
        /// <param name="size">The bytes size.</param>
        /// <returns></returns>
        inline uintptr_t Sig(const std::string& pattern);

        /// <summary>
        /// Memory scan.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="pattern">The pattern to scan.</param>
        /// <param name="size">The bytes size.</param>
        /// <returns></returns>
        uintptr_t Sig(const std::string& moduleName, const std::string& pattern);

        /// <summary>
        /// Memory scan all results.
        /// </summary>
        /// <param name="moduleName">The module name.</param>
        /// <param name="pattern">The pattern to scan.</param>
        /// <param name="first">Should only scan until the first result.</param>
        /// <returns></returns>
        std::vector<uintptr_t> Sig(const std::string& moduleName, const std::string& pattern, bool first);
	};
}

#pragma once
#include "Windows/Base.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Signature class.
	/// </summary>
	class API Signature
	{
	public:
		uintptr_t Address = 0;
		std::string Module;
		std::string Pattern;

		/// <summary>
		/// Create a new signature from an address.
		/// </summary>
		/// <param name="address">The address.</param>
		Signature(uintptr_t address);
		~Signature() = default;

		/// <summary>
		/// Create a new signature from HMODULE.
		/// </summary>
		/// <param name="hModule">The module handle.</param>
		Signature(HMODULE hModule);

		/// <summary>
		/// Create a new signature from FARPROC.
		/// </summary>
		/// <param name="function">The function.</param>
		Signature(FARPROC function);

		/// <summary>
		/// Create a new signature and scan the pattern.
		/// </summary>
		/// <param name="pattern">The pattern to scan.</param>
		Signature(const std::string& pattern);

		/// <summary>
		/// Create a new signature and scan the pattern.
		/// </summary>
		/// <param name="moduleName">The module name.</param>
		/// <param name="pattern">The pattern to scan.</param>
		Signature(const std::string& moduleName, const std::string& pattern);

		/// <summary>
		/// Add an offset to the address.
		/// </summary>
		/// <param name="offset">The offset.</param>
		/// <returns></returns>
		Signature& Offset(uintptr_t offset);

		/// <summary>
		/// Deref the address.
		/// </summary>
		/// <returns></returns>
		Signature& DeRef();

		/// <summary>
		/// Memory scan all results.
		/// </summary>
		/// <param name="first">Stop scanning on the first result.</param>
		/// <returns></returns>
		std::vector<uintptr_t> ScanAll(bool first = false);

		/// <summary>
		/// Get the address.
		/// </summary>
		inline operator uintptr_t()
		{
			return Address;
		}

		/// <summary>
		/// Cast to pointer type.
		/// </summary>
		template <typename T>
		inline operator T*()
		{
			return reinterpret_cast<T*>(Address);
		}

		/// <summary>
		/// Cast to reference type.
		/// </summary>
		template <typename T>
		inline operator T&()
		{
			return *reinterpret_cast<T*>(Address);
		}

	private:
		/// <summary>
		/// Create a new signature.
		/// </summary>
		Signature() = default;

		/// <summary>
		/// Memory scan.
		/// </summary
		/// <returns></returns>
		uintptr_t Scan();
	};
}

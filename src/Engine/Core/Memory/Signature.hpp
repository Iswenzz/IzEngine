#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API Signature
	{
	public:
		uintptr_t Address = 0;
		std::string Module;
		std::string Pattern;

		Signature(uintptr_t address);
		Signature(const std::string& pattern);
		Signature(const std::string& moduleName, const std::string& pattern);
		~Signature() = default;

		Signature& Offset(uintptr_t offset);
		Signature& DeRef();
		std::vector<uintptr_t> ScanAll(bool first = false);

		operator uintptr_t()
		{
			return Address;
		}

		template <typename T>
		operator T*()
		{
			return reinterpret_cast<T*>(Address);
		}

		template <typename T>
		operator T&()
		{
			return *reinterpret_cast<T*>(Address);
		}

	private:
		Signature() = default;
		uintptr_t Scan();
	};
}

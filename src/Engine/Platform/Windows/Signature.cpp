#include "Base.hpp"

#include "Engine/Core/Memory/Memory.hpp"
#include "Engine/Core/Memory/Signature.hpp"

#include <Psapi.h>

namespace IzEngine
{
	Signature::Signature(uintptr_t address) : Address(address) { }
	Signature::Signature(const std::string& pattern) : Signature("", pattern) { }

	Signature::Signature(const std::string& moduleName, const std::string& pattern)
	{
		Module = moduleName;
		Pattern = pattern;
		Address = Scan();
	}

	Signature& Signature::Offset(uintptr_t offset)
	{
		Address += offset;
		return *this;
	}

	Signature& Signature::DeRef()
	{
		if (Address)
			Address = *reinterpret_cast<uintptr_t*>(Address);
		return *this;
	}

	uintptr_t Signature::Scan()
	{
		std::vector<uintptr_t> addresses = ScanAll(true);
		return addresses.size() ? addresses.back() : 0;
	}

	std::vector<uintptr_t> Signature::ScanAll(bool first)
	{
		std::vector<uintptr_t> addresses;
		HMODULE hModule = GetModuleHandle(Module.empty() ? nullptr : Module.c_str());

		if (!hModule)
			return addresses;

		std::string bytes = Memory::Pattern(Pattern);
		const size_t size = bytes.size();
		if (!size)
			return addresses;

		MODULEINFO moduleInfo = { 0 };
		if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo)))
			return addresses;

		uintptr_t moduleBase = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
		uintptr_t moduleEnd = moduleBase + moduleInfo.SizeOfImage;

		// Walk region by region and skip anything not readable: touching a
		// PAGE_NOACCESS or guard page while scanning is itself a crash.
		uintptr_t address = moduleBase;
		while (address < moduleEnd - size)
		{
			MEMORY_BASIC_INFORMATION mbi = { 0 };
			if (!VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi)))
				break;

			const uintptr_t regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
			const bool readable =
				mbi.State == MEM_COMMIT && !(mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) && mbi.Protect != 0;

			if (!readable)
			{
				address = regionEnd;
				continue;
			}
			const uintptr_t scanEnd = std::min(regionEnd, moduleEnd) - size;
			for (; address <= scanEnd; ++address)
			{
				size_t i = 0;
				for (; i < size; ++i)
				{
					if (bytes[i] == '?')
						continue;

					if (bytes[i] != *reinterpret_cast<char*>(address + i))
						break;
				}
				if (i == size)
				{
					addresses.push_back(address);
					if (first)
						return addresses;
				}
			}
			address = regionEnd;
		}
		return addresses;
	}
}

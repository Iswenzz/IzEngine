#include "Base.hpp"

#include "Engine/Core/Memory/Memory.hpp"
#include "Engine/Core/Memory/Signature.hpp"

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

	// A signature resolves only when it matches exactly once. One loose enough to match twice would
	// otherwise land on whichever copy came first; a miss leaves the caller with 0, which it can test.
	uintptr_t Signature::Scan()
	{
		const std::vector<uintptr_t> addresses = ScanAll(Module, Pattern);
		if (addresses.size() > 1)
		{
			Log::WriteLine(Channel::Error, "Signature matches {} places in {}; ignoring it: {}", addresses.size(),
				Module.empty() ? "the executable" : Module, Pattern);
		}
		return addresses.size() == 1 ? addresses.front() : 0;
	}

	// Every match in the module's executable sections. Code only: it is quick, and data that happens
	// to look like an instruction cannot make a signature ambiguous.
	std::vector<uintptr_t> Signature::ScanAll(const std::string& moduleName, const std::string& pattern)
	{
		std::vector<uintptr_t> addresses;
		const HMODULE module = GetModuleHandle(moduleName.empty() ? nullptr : moduleName.c_str());
		const std::vector<int> bytes = Memory::Pattern(pattern);

		if (!module || bytes.empty())
			return addresses;

		const uintptr_t base = reinterpret_cast<uintptr_t>(module);
		const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
		const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
		const IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);

		for (int s = 0; s < nt->FileHeader.NumberOfSections; s++, section++)
		{
			if (!(section->Characteristics & IMAGE_SCN_MEM_EXECUTE) || section->Misc.VirtualSize < bytes.size())
				continue;

			const auto* code = reinterpret_cast<const uint8_t*>(base + section->VirtualAddress);
			const size_t last = section->Misc.VirtualSize - bytes.size();

			for (size_t at = 0; at <= last; at++)
			{
				size_t i = 0;
				while (i < bytes.size() && (bytes[i] < 0 || code[at + i] == bytes[i]))
					i++;

				if (i == bytes.size())
					addresses.push_back(reinterpret_cast<uintptr_t>(code + at));
			}
		}
		return addresses;
	}
}

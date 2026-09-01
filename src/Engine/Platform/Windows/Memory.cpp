#include "Base.hpp"

#include "Engine/Core/Memory/Memory.hpp"

namespace IzEngine
{
	void Memory::Read(uintptr_t address, void* data, size_t size)
	{
		IZ_ASSERT(address, "Address nullptr.");

		DWORD oldProtect;
		LPVOID lpAddress = reinterpret_cast<LPVOID>(address);

		if (!VirtualProtect(lpAddress, size, PAGE_EXECUTE_READ, &oldProtect))
			return;

		memcpy(data, lpAddress, size);
		VirtualProtect(lpAddress, size, oldProtect, &oldProtect);
	}

	void Memory::Write(uintptr_t address, const std::string& bytes)
	{
		IZ_ASSERT(address, "Address nullptr.");

		DWORD oldProtect;
		LPVOID lpAddress = reinterpret_cast<LPVOID>(address);
		size_t size = bytes.size();

		if (!VirtualProtect(lpAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect))
			return;

		memcpy(lpAddress, bytes.data(), size);
		VirtualProtect(lpAddress, size, oldProtect, &oldProtect);
		FlushInstructionCache(GetCurrentProcess(), lpAddress, size);
	}

	void Memory::Write(uintptr_t address, const std::vector<uint8_t>& bytes)
	{
		IZ_ASSERT(address, "Address nullptr.");

		DWORD oldProtect;
		LPVOID lpAddress = reinterpret_cast<LPVOID>(address);
		size_t size = bytes.size();

		if (!VirtualProtect(lpAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect))
			return;

		memcpy(lpAddress, bytes.data(), size);
		VirtualProtect(lpAddress, size, oldProtect, &oldProtect);
		FlushInstructionCache(GetCurrentProcess(), lpAddress, size);
	}

	void Memory::NOP(uintptr_t address, int size)
	{
		Write(address, std::string(size, '\x90'));
	}

	void Memory::JMP(uintptr_t address, uintptr_t to, int size)
	{
		IZ_ASSERT(size >= 5, "JMP needs at least 5 bytes.");

		const intptr_t delta = static_cast<intptr_t>(to) - static_cast<intptr_t>(address + 5);
		IZ_ASSERT(delta >= INT32_MIN && delta <= INT32_MAX, "JMP target out of rel32 range.");

		const int32_t rel = static_cast<int32_t>(delta);
		std::string bytes = "\xE9";
		bytes.append(reinterpret_cast<const char*>(&rel), sizeof(rel));

		NOP(address, size);
		Write(address, bytes);
	}

	void Memory::JMPABS(uintptr_t address, uintptr_t to, int size)
	{
#ifdef PLATFORM_64
		IZ_ASSERT(size >= 14, "JMPABS needs at least 14 bytes.");

		std::string bytes("\xFF\x25\x00\x00\x00\x00", 6);
		bytes.append(reinterpret_cast<const char*>(&to), sizeof(to));

		NOP(address, size);
		Write(address, bytes);
#else
		JMP(address, to, size);
#endif
	}

	void Memory::CALL(uintptr_t address, uintptr_t to, int size)
	{
		IZ_ASSERT(size >= 5, "CALL needs at least 5 bytes.");

		const intptr_t delta = static_cast<intptr_t>(to) - static_cast<intptr_t>(address + 5);
		IZ_ASSERT(delta >= INT32_MIN && delta <= INT32_MAX, "CALL target out of rel32 range.");

		const int32_t rel = static_cast<int32_t>(delta);
		std::string bytes = "\xE8";
		bytes.append(reinterpret_cast<const char*>(&rel), sizeof(rel));

		NOP(address, size);
		Write(address, bytes);
	}

	// The six byte middle ground between CALL and JMPABS: an indirect call through a pointer parked
	// close enough for the operand to reach it, so the target is absolute where CALL's rel32 would
	// truncate. A call site with live code behind it rarely has the fourteen bytes JMPABS wants.
	bool Memory::CALLPTR(uintptr_t address, uintptr_t to, int size)
	{
		IZ_ASSERT(size >= 6, "CALLPTR needs at least 6 bytes.");

		auto* slot = static_cast<uintptr_t*>(Reserve(address, sizeof(uintptr_t)));
		if (!slot)
			return false;

		*slot = to;

		const intptr_t delta = reinterpret_cast<intptr_t>(slot) - static_cast<intptr_t>(address + 6);
		if (delta < INT32_MIN || delta > INT32_MAX)
			return false;

		const int32_t rel = static_cast<int32_t>(delta);
		std::string bytes("\xFF\x15", 2);
		bytes.append(reinterpret_cast<const char*>(&rel), sizeof(rel));

		NOP(address, size);
		Write(address, bytes);
		return true;
	}

	// Memory within a rel32 operand's reach of the anchor. One region is kept per anchor and carved up:
	// VirtualAlloc reserves at allocation granularity, so a region per pointer would spend 64 KB on
	// eight bytes. Reservations are permanent - they back patched code that lives as long as the process.
	void* Memory::Reserve(uintptr_t anchor, size_t size)
	{
		struct Region
		{
			uintptr_t Base;
			size_t Used;
			size_t Size;
		};
		static std::vector<Region> regions;

		size = (size + sizeof(uintptr_t) - 1) & ~(sizeof(uintptr_t) - 1);

		// Half the range on each side, so a slot handed out now still reaches an anchor patched later.
		const auto reaches = [anchor](uintptr_t at)
		{
			const intptr_t delta = static_cast<intptr_t>(at) - static_cast<intptr_t>(anchor);
			return delta >= INT32_MIN / 2 && delta <= INT32_MAX / 2;
		};

		for (Region& region : regions)
		{
			if (region.Size - region.Used < size || !reaches(region.Base + region.Used))
				continue;

			void* slot = reinterpret_cast<void*>(region.Base + region.Used);
			region.Used += size;
			return slot;
		}

		SYSTEM_INFO info = {};
		GetSystemInfo(&info);

		const uintptr_t granularity = info.dwAllocationGranularity;
		const uintptr_t window = 0x7FFF0000;
		uintptr_t at = anchor > window ? anchor - window : granularity;

		// VirtualAlloc answers at or above the address asked for, so the search walks up from the bottom
		// of the window until a free region lands inside it.
		for (at -= at % granularity; at < anchor; at += granularity)
		{
			void* page
				= VirtualAlloc(reinterpret_cast<void*>(at), granularity, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (!page)
				continue;

			regions.push_back({ reinterpret_cast<uintptr_t>(page), size, granularity });
			return page;
		}
		return nullptr;
	}

	void** Memory::FindImport(uintptr_t base, const char* dllName, const char* funcName)
	{
		IZ_ASSERT(base, "Base nullptr.");

		const auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
		if (dos->e_magic != IMAGE_DOS_SIGNATURE)
			return nullptr;

		const auto nt = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
		if (nt->Signature != IMAGE_NT_SIGNATURE)
			return nullptr;

		const auto dir = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		if (!dir.VirtualAddress)
			return nullptr;

		auto imp = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base + dir.VirtualAddress);
		for (; imp->Name; ++imp)
		{
			if (_stricmp(reinterpret_cast<const char*>(base + imp->Name), dllName) != 0)
				continue;

			const auto names = imp->OriginalFirstThunk ? imp->OriginalFirstThunk : imp->FirstThunk;
			auto thunk = reinterpret_cast<IMAGE_THUNK_DATA*>(base + names);
			auto iat = reinterpret_cast<IMAGE_THUNK_DATA*>(base + imp->FirstThunk);

			for (; thunk->u1.AddressOfData; ++thunk, ++iat)
			{
				if (thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
					continue;

				const auto name = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base + thunk->u1.AddressOfData);
				if (strcmp(reinterpret_cast<const char*>(name->Name), funcName) == 0)
					return reinterpret_cast<void**>(&iat->u1.Function);
			}
		}
		return nullptr;
	}

	void* Memory::PatchImport(uintptr_t base, const char* dllName, const char* funcName, void* detour)
	{
		void** slot = FindImport(base, dllName, funcName);
		if (!slot)
			return nullptr;

		void* original = *slot;
		Set<void*>(reinterpret_cast<uintptr_t>(slot), detour);
		return original;
	}

	uintptr_t Memory::Swap(uintptr_t value)
	{
		uintptr_t result = 0;
		for (size_t i = 0; i < sizeof(value); ++i)
			result |= ((value >> (i * 8)) & 0xFF) << ((sizeof(value) - 1 - i) * 8);
		return result;
	}

	uintptr_t Memory::LE(uintptr_t value)
	{
		if constexpr (std::endian::native == std::endian::little)
			return value;
		else
			return Swap(value);
	}

	uintptr_t Memory::BE(uintptr_t value)
	{
		if constexpr (std::endian::native == std::endian::big)
			return value;
		else
			return Swap(value);
	}

	std::string Memory::Pattern(const std::string& pattern)
	{
		std::string result;
		for (size_t i = 0; i < pattern.size(); ++i)
		{
			if (pattern[i] == ' ')
				continue;

			// Support both "?" and "??" for a wildcard byte.
			if (pattern[i] == '?')
			{
				result += '?';
				if (i + 1 < pattern.size() && pattern[i + 1] == '?')
					++i;
				continue;
			}
			const std::string byte = pattern.substr(i, 2);
			char* end = nullptr;
			const long value = std::strtol(byte.c_str(), &end, 16);
			if (end == byte.c_str())
			{
				Log::WriteLine(Channel::Error, "Invalid byte pattern: {}", pattern);
				return {};
			}
			result.append(1, static_cast<char>(value));
			++i;
		}
		return result;
	}
}

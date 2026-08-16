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

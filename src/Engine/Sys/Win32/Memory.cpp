#pragma warning(push)
#pragma warning(disable: 6387)
#include "Memory.hpp"

namespace IW3SR::Engine
{
    void Memory::Read(uintptr_t address, void* data, int size)
    {
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(address), data, size, nullptr);
    }

    void Memory::Write(uintptr_t address, const char* bytes, int size)
    {
        DWORD oldProtect;
        LPVOID lpAddress = reinterpret_cast<LPVOID>(address);

        VirtualProtect(lpAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(lpAddress, bytes, size);
        VirtualProtect(lpAddress, size, oldProtect, nullptr);
    }

    uintptr_t Memory::Scan(std::string moduleName, const char* bytes, size_t size)
    {
        std::vector<uintptr_t> addresses = ScanAll(moduleName, bytes, size, true);
        return addresses.size() ? addresses.back() : 0;
    }

    std::vector<uintptr_t> Memory::ScanAll(std::string moduleName, const char* bytes, size_t size, bool first)
    {
        std::vector<uintptr_t> addresses;
        HMODULE hModule = GetModuleHandleA(moduleName.c_str());

        if (!hModule) return addresses;

        MODULEINFO moduleInfo;
        GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo));

        uintptr_t moduleBase = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
        uintptr_t moduleEnd = moduleBase + moduleInfo.SizeOfImage;

        std::vector<unsigned char> buffer(moduleInfo.SizeOfImage);
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(moduleBase),
            &buffer[0], moduleInfo.SizeOfImage, nullptr);

        for (uintptr_t address = moduleBase; address < moduleEnd - size; ++address)
        {
            if (!memcmp(&buffer[address - moduleBase], bytes, size))
            {
                addresses.push_back(address);
                if (first) return addresses;
            }
        }
        return addresses;
    }

    void Memory::NOP(uintptr_t address, int size)
    {
        std::string bytes(size, '\x90');
        Write(address, bytes.c_str(), size);
    }

    void Memory::JMP(uintptr_t address, uintptr_t to, int size)
    {
        std::string bytes = "\xE9";
        uintptr_t nearAddress = to - (address + 5);
        bytes.append(reinterpret_cast<char*>(&nearAddress), sizeof(to));

        NOP(address, size);
        Write(address, bytes.c_str(), 5);
    }

    void Memory::CALL(uintptr_t address, uintptr_t to, int size)
    {
        std::string bytes = "\xE8";
        uintptr_t nearAddress = to - (address + 5);
        bytes.append(reinterpret_cast<char*>(&nearAddress), sizeof(to));

        NOP(address, size);
        Write(address, bytes.c_str(), 5);
    }

    uintptr_t Memory::LE(uintptr_t value)
    {
        uintptr_t result = 0;
        for (int i = 0; i < sizeof(value); ++i)
            result |= ((value >> (i * 8)) & 0xFF) << (24 - i * 8);
        return result;
    }

    uintptr_t Memory::BE(uintptr_t value)
    {
        uintptr_t result = 0;
        for (int i = 0; i < sizeof(value); ++i)
            result |= ((value >> (24 - i * 8)) & 0xFF) << (i * 8);
        return result;
    }
}

#pragma warning(pop)

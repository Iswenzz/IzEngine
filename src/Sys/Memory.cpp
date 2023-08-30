#include "Memory.hpp"

#include <Windows.h>
#include <Psapi.h>

namespace IW3SR
{
    void Memory::Write(uint32_t address, std::string bytes, int size)
    {
        DWORD oldProtect;
        LPVOID lpAddress = reinterpret_cast<LPVOID>(address);

        VirtualProtect(lpAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(lpAddress, reinterpret_cast<const void*>(bytes.c_str()), size);
        VirtualProtect(lpAddress, size, oldProtect, NULL);
    }

    uint32_t Memory::Scan(std::string moduleName, std::string bytes, size_t size)
    {
        std::vector<uint32_t> addresses = ScanAll(moduleName, bytes, size, true);
        return addresses.size() ? addresses.back() : 0;
    }

    std::vector<uint32_t> Memory::ScanAll(std::string moduleName, std::string bytes, size_t size, bool first)
    {
        std::vector<uint32_t> addresses;
        HMODULE hModule = GetModuleHandleA(moduleName.c_str());

        if (!hModule) return addresses;

        MODULEINFO moduleInfo;
        GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo));

        uint32_t moduleBase = reinterpret_cast<uint32_t>(moduleInfo.lpBaseOfDll);
        uint32_t moduleEnd = moduleBase + moduleInfo.SizeOfImage;

        std::vector<unsigned char> buffer(moduleInfo.SizeOfImage);
        ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPCVOID>(moduleBase),
            &buffer[0], moduleInfo.SizeOfImage, NULL);

        for (uint32_t address = moduleBase; address < moduleEnd - size; ++address)
        {
            if (!memcmp(&buffer[address - moduleBase], bytes.c_str(), size))
            {
                addresses.push_back(address);
                if (first) return addresses;
            }
        }
        return addresses;
    }

    void Memory::NOP(uint32_t address, int size)
    {
        std::string bytes(size, '\x90');
        Write(address, bytes, size);
    }

    void Memory::JMP(uint32_t address, uint32_t to, int size)
    {
        std::string bytes = "\xE9";
        uint32_t nearAddress = to - (address + 5);
        bytes.append(reinterpret_cast<char*>(&nearAddress), sizeof(to));

        NOP(address, size);
        Write(address, bytes, 5);
    }

    uint32_t Memory::LE(uint32_t value)
    {
        uint32_t result = 0;
        for (int i = 0; i < sizeof(value); ++i)
            result |= ((value >> (i * 8)) & 0xFF) << (24 - i * 8);
        return result;
    }

    uint32_t Memory::BE(uint32_t value)
    {
        uint32_t result = 0;
        for (int i = 0; i < sizeof(value); ++i)
            result |= ((value >> (24 - i * 8)) & 0xFF) << (i * 8);
        return result;
    }
}

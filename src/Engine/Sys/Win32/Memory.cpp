#include "Memory.hpp"

#pragma warning(push)
#pragma warning(disable: 6387)

namespace IW3SR::Engine
{
    void Memory::Read(uintptr_t address, void* data, int size)
    {
        DWORD oldProtect;
        LPVOID lpAddress = reinterpret_cast<LPVOID>(address);

        VirtualProtect(lpAddress, size, PAGE_READONLY, &oldProtect);
        memcpy(data, lpAddress, size);
        VirtualProtect(lpAddress, size, oldProtect, nullptr);
    }

    void Memory::Write(uintptr_t address, const std::string& pattern)
    {
        WriteBytes(address, HexToBytes(pattern));
    }

    void Memory::WriteBytes(uintptr_t address, const std::string& bytes)
    {
        if (!address) return;

        DWORD oldProtect;
        LPVOID lpAddress = reinterpret_cast<LPVOID>(address);
        int size = bytes.size();

        VirtualProtect(lpAddress, size, PAGE_READWRITE, &oldProtect);
        memcpy(lpAddress, bytes.data(), size);
        VirtualProtect(lpAddress, size, oldProtect, nullptr);
    }

    void Memory::NOP(uintptr_t address, int size)
    {
        WriteBytes(address, std::string(size, '\x90'));
    }

    void Memory::JMP(uintptr_t address, uintptr_t to, int size)
    {
        std::string bytes = "\xE9";
        uintptr_t nearAddress = to - (address + 5);
        bytes.append(reinterpret_cast<char*>(&nearAddress), sizeof(to));

        NOP(address, size);
        WriteBytes(address, bytes);
    }

    void Memory::CALL(uintptr_t address, uintptr_t to, int size)
    {
        std::string bytes = "\xE8";
        uintptr_t nearAddress = to - (address + 5);
        bytes.append(reinterpret_cast<char*>(&nearAddress), sizeof(to));

        NOP(address, size);
        WriteBytes(address, bytes);
    }

    constexpr uintptr_t Memory::LE(uintptr_t value)
    {
        uintptr_t result = 0;
        for (int i = 0; i < sizeof(value); ++i)
            result |= ((value >> (i * 8)) & 0xFF) << (24 - i * 8);
        return result;
    }

    constexpr uintptr_t Memory::BE(uintptr_t value)
    {
        uintptr_t result = 0;
        for (int i = 0; i < sizeof(value); ++i)
            result |= ((value >> (24 - i * 8)) & 0xFF) << (i * 8);
        return result;
    }

    constexpr std::string Memory::HexToBytes(const std::string& pattern)
    {
        std::string result;
        for (size_t i = 0; i < pattern.size(); ++i)
        {
            if (pattern[i] == ' ')
                continue;

            if (pattern[i] == '?')
            {
                result += '?';
                ++i;
                continue;
            }
            std::string byte = pattern.substr(i, 2);
            result.append(1, std::stoi(byte, nullptr, 16));
            ++i;
        }
        return result;
    }
}

#pragma warning(pop)

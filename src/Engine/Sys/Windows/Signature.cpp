#include "Signature.hpp"
#include "Memory.hpp"

#include <Psapi.h>

namespace IW3SR::Engine
{
    Signature::Signature(uintptr_t address) : Address(address) { }
    Signature::Signature(HMODULE hModule) : Address(reinterpret_cast<uintptr_t>(hModule)) { }
    Signature::Signature(FARPROC function) : Address(reinterpret_cast<uintptr_t>(function)) { }
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
        HMODULE hModule = GetModuleHandle(Module.c_str());
        if (!hModule) return addresses;

        std::string bytes = Memory::HexToBytes(Pattern);
        int size = bytes.size();

        MODULEINFO moduleInfo;
        GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo));
        uintptr_t moduleBase = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
        uintptr_t moduleEnd = moduleBase + moduleInfo.SizeOfImage;

        for (uintptr_t address = moduleBase; address < moduleEnd - size; ++address)
        {
            int i = 0;
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
                if (first) return addresses;
            }
        }
        return addresses;
    }
}

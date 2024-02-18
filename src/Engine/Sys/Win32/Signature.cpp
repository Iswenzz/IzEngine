#include "Signature.hpp"

namespace IW3SR::Engine
{
    Signature::Signature(uintptr_t address) : Address(address) { }
    Signature::Signature(uintptr_t base, uintptr_t address) : Address(base + address) { }
    Signature::Signature(HMODULE hModule) : Address(reinterpret_cast<uintptr_t>(hModule)) { }
    Signature::Signature(FARPROC function) : Address(reinterpret_cast<uintptr_t>(function)) { }
    Signature::Signature(const std::string& pattern) : Signature("", pattern) { }

    Signature::Signature(const std::string& moduleName, const std::string& pattern)
    {
        Address = Scan(moduleName, pattern);
    }

    uintptr_t Signature::Scan(const std::string& pattern)
    {
        return Scan("", pattern);
    }

    uintptr_t Signature::Scan(const std::string& moduleName, const std::string& pattern)
    {
        std::vector<uintptr_t> addresses = ScanAll(moduleName, pattern, true);
        if (!addresses.size())
            throw std::runtime_error("Scan couldn't find any matches.");
        return addresses.back();
    }

    std::vector<uintptr_t> Signature::ScanAll(const std::string& moduleName, const std::string& pattern, bool first)
    {
        std::vector<uintptr_t> addresses;
        HMODULE hModule = GetModuleHandle(moduleName.c_str());
        if (!hModule) return addresses;

        std::string bytes = Memory::HexToBytes(pattern);
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

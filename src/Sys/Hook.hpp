#pragma once
#include <Windows.h>
#include <MinHook.h>

namespace IW3SR
{
    /// <summary>
    /// Hook class.
    /// </summary>
    /// <typeparam name="T">The hook function definition type.</typeparam>
    template <typename T>
    class Hook
    {
    public:
        uint32_t Address = 0;
        uint32_t Detour = 0;
        uint32_t Trampoline = 0;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        Hook() = default;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        /// <param name="address">The address of the target.</param>
        /// <param name="detour">The detour function.</param>
        Hook(uint32_t address, T detour)
        {
            Address = address;
            Detour = reinterpret_cast<uint32_t>(detour);

            Install();
        }

        /// <summary>
        /// Install the hook and returns the trampoline address.
        /// </summary>
        /// <returns></returns>
        T Install()
        {
            LPVOID lpAddress = reinterpret_cast<LPVOID>(Address);
            LPVOID lpDetour = reinterpret_cast<LPVOID>(Detour);
            LPVOID lpTrampoline = nullptr;

            MH_CreateHook(lpAddress, lpDetour, &lpTrampoline);
            Enable();

            Trampoline = reinterpret_cast<uint32_t>(lpTrampoline);
            return reinterpret_cast<T>(lpTrampoline);
        }

        /// <summary>
        /// Remove the hook.
        /// </summary>
        void Remove()
        {
            MH_RemoveHook(reinterpret_cast<LPVOID>(Address));
        }

        /// <summary>
        /// Enable the hook.
        /// </summary>
        void Enable()
        {
            MH_EnableHook(reinterpret_cast<LPVOID>(Address));
        }

        /// <summary>
        /// Disable the hook.
        /// </summary>
        void Disable()
        {
            MH_DisableHook(reinterpret_cast<LPVOID>(Address));
        }

        /// <summary>
        /// Call the trampoline function.
        /// </summary>
        /// <typeparam name="R">The return type.</typeparam>
        /// <typeparam name="...Args">The function args type.</typeparam>
        /// <param name="...args">The function args.</param>
        /// <returns></returns>
        template <typename R = void, typename ...Args>
        R Call(const Args... args)
        {
            return reinterpret_cast<T>(Trampoline)(args...);
        }
    };
}

#pragma once
#include <vector>
#include <map>
#include <cstdint>

#include <Windows.h>
#include <polyhook2/Detour/x86Detour.hpp>

namespace IW3SR
{
    /// <summary>
    /// Hook class.
    /// </summary>
    /// <typeparam name="T">The hook function definition type.</typeparam>
    template <typename T>
    class Hook : public PLH::x86Detour
    {
    public:
        uintptr_t Address = 0;
        uintptr_t Trampoline = 0;
        T Detour = 0;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        Hook() = default;
        virtual ~Hook() = default;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        /// <param name="target">The target address.</param>
        /// <param name="detour">The detour function.</param>
        Hook(uintptr_t target, T detour) : PLH::x86Detour(
            static_cast<uint64_t>(target), 
            reinterpret_cast<uint64_t>(detour),
            reinterpret_cast<uint64_t*>(&Trampoline))
        {
            Address = target;
            Detour = detour;

            Install();
        }

        /// <summary>
        /// Install the hook.
        /// </summary>
        /// <returns></returns>
        void Install()
        {
            hook();
        }

        /// <summary>
        /// Remove the hook.
        /// </summary>
        void Remove()
        {
            unHook();
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

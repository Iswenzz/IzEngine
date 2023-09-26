#pragma once
#include <functional>
#include <polyhook2/Detour/x86Detour.hpp>

namespace IW3SR
{
    /// <summary>
    /// Hook class.
    /// </summary>
    /// <typeparam name="T">The hook function definition.</typeparam>
    template <typename T>
    class Hook : public PLH::x86Detour
    {
        using R = typename std::function<T>::result_type;
    public:
        uintptr_t Address = 0;
        std::function<T> Detour = nullptr;
        std::function<T> Trampoline = nullptr;

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
            Detour = std::function<T>(reinterpret_cast<T*>(detour));

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
        /// <typeparam name="...Args">The function args.</typeparam>
        /// <param name="...args">The function args.</param>
        /// <returns></returns>
        template <typename ...Args>
        R operator()(Args&& ...args)
        {
            return Trampoline(args...);
        }
    };
}

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
    class Hook
    {
        using R = typename std::function<T>::result_type;
    public:
        uint64_t Address = 0;
        uint64_t Callback = 0;
        std::function<T> Trampoline = nullptr;
        std::unique_ptr<PLH::x86Detour> Detour;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        Hook() = default;
        ~Hook() = default;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        /// <param name="target">The target function.</param>
        /// <param name="callback">The callback function.</param>
        Hook(T target, T callback) : Hook(reinterpret_cast<uintptr_t>(target), callback) { }

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        /// <param name="target">The target address.</param>
        /// <param name="callback">The callback function.</param>
        Hook(uintptr_t target, T callback)
        {
            Address = target;
            Callback = reinterpret_cast<uint64_t>(callback);
        }

        /// <summary>
        /// Install the hook.
        /// </summary>
        /// <returns></returns>
        void Install()
        {
            if (Detour && Detour->isHooked())
                return;

            uint64_t trampoline = 0;
            Detour = std::make_unique<PLH::x86Detour>(Address, Callback, &trampoline);
            Detour->hook();
            Trampoline = std::function<T>(reinterpret_cast<T*>(trampoline));
        }

        /// <summary>
        /// Remove the hook.
        /// </summary>
        void Remove()
        {
            if (!Detour || !Detour->isHooked())
                return;

            Detour->unHook();
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

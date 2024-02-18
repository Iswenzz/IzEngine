#pragma once
#include <functional>
#include <polyhook2/Detour/NatDetour.hpp>

namespace IW3SR::Engine
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
        uint64_t Trampoline = 0;
        std::function<T> Original = nullptr;
        std::unique_ptr<PLH::NatDetour> Detour = nullptr;
        bool IsEnabled = false;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        /// <param name="callback">The callback function.</param>
        Hook(T callback) : Hook(0ull, callback) { }
        ~Hook() = default;

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        /// <param name="target">The target function.</param>
        /// <param name="callback">The callback function.</param>
        Hook(T target, T callback) : Hook(reinterpret_cast<uint64_t>(target), callback) { }

        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        /// <param name="target">The target address.</param>
        /// <param name="callback">The callback function.</param>
        Hook(uint64_t target, T callback)
        {
            Address = target;
            Callback = reinterpret_cast<uint64_t>(callback);
        }

        /// <summary>
        /// Assign address.
        /// </summary>
        /// <param name="address">The target address.</param>
        /// <returns></returns>
        void operator <(uintptr_t address)
        {
            Address = address;
        }

        /// <summary>
        /// Install the hook.
        /// </summary>
        /// <returns></returns>
        void Install()
        {
            if (IsEnabled) return;
            IsEnabled = true;

            Detour = std::make_unique<PLH::NatDetour>(Address, Callback, &Trampoline);
            Detour->hook();
            Original = std::function<T>(reinterpret_cast<T*>(Trampoline));
        }

        /// <summary>
        /// Remove the hook.
        /// </summary>
        void Remove()
        {
            if (!IsEnabled) return;
            IsEnabled = false;

            Detour->unHook();
            Detour.reset();
        }

        /// <summary>
        /// Call the trampoline function.
        /// </summary>
        /// <typeparam name="...Args">The function args.</typeparam>
        /// <param name="...args">The function args.</param>
        /// <returns></returns>
        template <typename ...Args>
        inline R operator()(Args&& ...args)
        {
            return Original(args...);
        }

        /// <summary>
        /// Is hook enabled.
        /// </summary>
        inline operator bool() const
        {
            return IsEnabled;
        }

    private:
        /// <summary>
        /// Initialize a new Hook instance.
        /// </summary>
        Hook() = default;
    };
}

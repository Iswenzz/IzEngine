#pragma once
#include "Engine/Core/Base.hpp"

#include <polyhook2/Detour/NatDetour.hpp>

#define VTABLE(instance, index) (*reinterpret_cast<uintptr_t**>(instance))[index]

namespace IzEngine
{
	template <typename T>
	class Hook
	{
		using R = typename std::function<T>::result_type;

	public:
		uint64_t Address = 0;
		uint64_t Callback = 0;
		uint64_t Trampoline = 0;
		T* Original = nullptr;
		Scope<PLH::NatDetour> Detour = nullptr;
		bool IsEnabled = false;

		Hook() = default;
		Hook(T callback) : Hook(0ull, callback) { }
		Hook(T address, T callback) : Hook(reinterpret_cast<uint64_t>(address), callback) { }
		Hook(uint64_t address, T callback) : Hook(address, reinterpret_cast<uint64_t>(callback)) { }
		Hook(uint64_t address, uint64_t callback) : Address(address), Callback(callback) { }

		~Hook()
		{
			Remove();
		}

		void Install()
		{
			IZ_ASSERT(Address, "Hook address is nullptr.");

			if (IsEnabled || !Address)
				return;

			IsEnabled = true;
			Detour = CreateScope<PLH::NatDetour>(Address, Callback, &Trampoline);
			Detour->hook();
			Original = reinterpret_cast<T*>(Trampoline);
		}

		void Update(uintptr_t address)
		{
			Remove();
			Address = address;
			Install();
		}

		void Remove()
		{
			if (!IsEnabled)
				return;
			IsEnabled = false;

			Detour->unHook();
			Detour.reset();
		}

		template <typename... Args>
		inline R operator()(Args&&... args)
		{
			return Original(args...);
		}

		inline operator bool() const
		{
			return IsEnabled;
		}
	};
}

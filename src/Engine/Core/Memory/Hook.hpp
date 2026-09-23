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
		std::array<uint8_t, 8> Pristine = {};
		std::array<uint8_t, 8> Patched = {};

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

			std::memcpy(Pristine.data(), reinterpret_cast<const void*>(Address), Pristine.size());

			Detour = CreateScope<PLH::NatDetour>(Address, Callback, &Trampoline);
			if (!Detour->hook() || !Trampoline)
			{
				Log::WriteLine(Channel::Error, "Failed to hook address {:#x}.", Address);
				Detour.reset();
				Trampoline = 0;
				return;
			}
			IsEnabled = true;
			Original = reinterpret_cast<T*>(Trampoline);
			std::memcpy(Patched.data(), reinterpret_cast<const void*>(Address), Patched.size());
		}

		// Rehooking in place writes stale bytes over whatever chained on top of us, so it only happens
		// once our patch is gone and the prologue is back to what it was, as after a module reload.
		void Update(uintptr_t address)
		{
			if (IsEnabled && address == Address)
			{
				const auto current = reinterpret_cast<const uint8_t*>(Address);
				const bool restored = Patched != Pristine && std::equal(Pristine.begin(), Pristine.end(), current);
				if (!restored)
					return;
			}

			Remove();
			Address = address;

			if (!Address)
			{
				Log::WriteLine(Channel::Error, "No address to retarget a hook to; it stays off.");
				return;
			}
			Install();
		}

		void Remove()
		{
			if (!IsEnabled)
				return;
			IsEnabled = false;

			Detour->unHook();
			Detour.reset();

			Trampoline = 0;
			Original = nullptr;
		}

		template <typename... Args>
		inline R operator()(Args&&... args)
		{
			IZ_ASSERT(Original, "Calling a hook that is not installed.");
			return Original(std::forward<Args>(args)...);
		}

		inline operator bool() const
		{
			return IsEnabled;
		}
	};
}

#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	template <typename T>
	class Function
	{
		using R = typename std::function<T>::result_type;

	public:
		uintptr_t Address = 0;
		T* Func = nullptr;

		Function() = default;
		Function(T function) : Function(reinterpret_cast<uintptr_t>(function)) { }
		Function(uintptr_t address) : Address(address), Func(reinterpret_cast<T*>(address)) { }
		~Function() = default;

		Function<T>& operator=(uintptr_t address)
		{
			Address = address;
			Func = reinterpret_cast<T*>(address);
			return *this;
		}

		void Update(uintptr_t address)
		{
			Address = address;
			Func = reinterpret_cast<T*>(address);
		}

		template <typename... Args>
		R operator()(Args&&... args)
		{
			return Func(args...);
		}

		inline operator bool() const
		{
			return Address;
		}
	};
}

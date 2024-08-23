#pragma once
#include <functional>

namespace IzEngine
{
	/// <summary>
	/// Function wrapper.
	/// </summary>
	/// <typeparam name="T">The function definition.</typeparam>
	template <typename T>
	class Function
	{
		using R = typename std::function<T>::result_type;

	public:
		uintptr_t Address = 0;
		T* Func = nullptr;

		/// <summary>
		/// Initialize a new function wrapper.
		/// </summary>
		Function() = default;
		~Function() = default;

		/// <summary>
		/// Initialize a new function wrapper.
		/// </summary>
		/// <param name="address">The target address.</param>
		Function(uintptr_t address) : Address(address), Func(reinterpret_cast<T*>(address)) { }

		/// <summary>
		/// Initialize a new function wrapper.
		/// </summary>
		/// <param name="function">The target function.</param>
		Function(T function) : Address(reinterpret_cast<uintptr_t>(function)), Func(function) { }

		/// <summary>
		/// Assign address.
		/// </summary>
		/// <param name="address">The target address.</param>
		/// <returns></returns>
		Function<T>& operator=(uintptr_t address)
		{
			Address = address;
			Func = reinterpret_cast<T*>(address);
			return *this;
		}

		/// <summary>
		/// Assign address.
		/// </summary>
		/// <param name="address">The target address.</param>
		/// <returns></returns>
		void operator<(uintptr_t address)
		{
			Address = address;
			Func = reinterpret_cast<T*>(address);
		}

		/// <summary>
		/// Call the function.
		/// </summary>
		/// <typeparam name="...Args">The function args.</typeparam>
		/// <param name="...args">The function args.</param>
		/// <returns></returns>
		template <typename... Args>
		R operator()(Args&&... args)
		{
			return Func(args...);
		}

		/// <summary>
		/// Is function defined.
		/// </summary>
		inline operator bool() const
		{
			return Address;
		}
	};
}

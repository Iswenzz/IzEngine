#pragma once
#include <functional>

namespace IW3SR
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
		int Address = 0;
		std::function<T> Func = nullptr;

		/// <summary>
		/// Initialize a new function wrapper.
		/// </summary>
		Function() = default;

		/// <summary>
		/// Initialize a new Function wrapper.
		/// </summary>
		/// <param name="address">The address of the function.</param>
		Function(const uintptr_t& address) : Address(address), Func(reinterpret_cast<T*>(address)) { }

		/// <summary>
		/// Assign function address.
		/// </summary>
		/// <param name="address">The function address.</param>
		/// <returns></returns>
		Function& operator=(const uintptr_t& address)
		{
			Address = address;
			Func = std::function<T>(reinterpret_cast<T*>(address));
			return *this;
		}

		/// <summary>
		/// Call the function.
		/// </summary>
		/// <typeparam name="...Args">The function args.</typeparam>
		/// <param name="...args">The function args.</param>
		/// <returns></returns>
		template <typename ...Args>
		R operator()(Args&& ...args)
		{
			return Func(args...);
		}

		/// <summary>
		/// Is function defined.
		/// </summary>
		operator bool() const
		{
			return Address;
		}
	};
}

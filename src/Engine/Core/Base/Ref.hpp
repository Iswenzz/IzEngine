#pragma once
#include <memory>

namespace IW3SR::Engine
{
	/// <summary>
	/// Create a unique pointer.
	/// </summary>
	/// <typeparam name="T">The type.</typeparam>
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	/// <summary>
	/// Create a shared pointer.
	/// </summary>
	/// <typeparam name="T">The type.</typeparam>
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

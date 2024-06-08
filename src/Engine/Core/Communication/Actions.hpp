#pragma once
#include "Engine/Base.hpp"

#include <functional>
#include <mutex>
#include <queue>

namespace IzEngine
{
	/// <summary>
	/// Atomic actions.
	/// </summary>
	class API Actions
	{
	public:
		/// <summary>
		/// Add action.
		/// </summary>
		/// <param name="func">The action function.</param>
		static void Add(const std::function<void()>& func);

		/// <summary>
		/// Submit actions.
		/// </summary>
		static void Submit();

	private:
		static inline std::queue<std::function<void()>> Queue;
		static inline std::mutex Mutex;
	};
}

#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API Actions
	{
	public:
		static void Add(const std::function<void()>& func);
		static void Submit();

	private:
		static inline std::queue<std::function<void()>> Queue;
		static inline std::mutex Mutex;
	};
}

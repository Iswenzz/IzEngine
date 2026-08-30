#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API Dispatcher
	{
	public:
		void Add(const std::function<void()>& func);
		void Submit();

	private:
		std::queue<std::function<void()>> Queue;
		std::mutex Mutex;
	};
}

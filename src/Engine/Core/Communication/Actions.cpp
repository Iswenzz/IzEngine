#include "Actions.hpp"

namespace IzEngine
{
	void Actions::Add(const std::function<void()>& func)
	{
		std::scoped_lock(Mutex);

		Queue.push(func);
	}

	void Actions::Submit()
	{
		std::scoped_lock(Mutex);

		while (!Queue.empty())
		{
			Queue.front()();
			Queue.pop();
		}
	}
}

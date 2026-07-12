#include "Actions.hpp"

namespace IzEngine
{
	void Actions::Add(const std::function<void()>& func)
	{
		std::scoped_lock lock(Mutex);

		Queue.push(func);
	}

	void Actions::Submit()
	{
		std::queue<std::function<void()>> pending;
		{
			std::scoped_lock lock(Mutex);
			pending.swap(Queue);
		}
		while (!pending.empty())
		{
			try
			{
				pending.front()();
			}
			catch (...)
			{
				Log::WriteLine(Channel::Error, "Action aborted");
			}
			pending.pop();
		}
	}
}

#include "Dispatcher.hpp"

namespace IzEngine
{
	void Dispatcher::Add(const std::function<void()>& func)
	{
		std::scoped_lock lock(Mutex);

		Queue.push(func);
	}

	void Dispatcher::Submit()
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
				Log::WriteLine(Channel::Error, "Dispatched task aborted");
			}
			pending.pop();
		}
	}
}

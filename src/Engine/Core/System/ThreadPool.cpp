#include "ThreadPool.hpp"

namespace IzEngine
{
	void ThreadPool::Initialize(int threads)
	{
		if (Running)
			return;

		if (threads < 1)
			threads = 2;

		Running = true;
		for (int i = 0; i < threads; i++)
			Threads.emplace_back(Worker);
	}

	void ThreadPool::Shutdown()
	{
		{
			std::scoped_lock lock(Mutex);
			Running = false;
		}
		Condition.notify_all();
		for (auto& thread : Threads)
			thread.join();
		Threads.clear();
	}

	void ThreadPool::Queue(std::function<void()> task)
	{
		IZ_ASSERT(Running, "ThreadPool is not active");
		{
			std::scoped_lock lock(Mutex);
			Tasks.push(std::move(task));
		}
		Condition.notify_one();
	}

	void ThreadPool::Worker()
	{
		while (true)
		{
			std::function<void()> task;
			{
				std::unique_lock lock(Mutex);
				Condition.wait(lock, [] { return !Tasks.empty() || !Running; });
				if (!Running && Tasks.empty())
					return;
				task = std::move(Tasks.front());
				Tasks.pop();
			}
			task();
		}
	}
}

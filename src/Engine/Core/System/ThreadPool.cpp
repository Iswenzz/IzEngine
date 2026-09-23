#include "ThreadPool.hpp"

namespace IzEngine
{
	WorkerThreads::~WorkerThreads()
	{
		for (auto& thread : Threads)
			if (thread.joinable())
				thread.detach();
	}

	void ThreadPool::Initialize(int threads)
	{
		if (Running)
			return;

		if (threads < 1)
			threads = 2;

		Running = true;
		for (int i = 0; i < threads; i++)
			Workers.Threads.emplace_back(Worker);
	}

	void ThreadPool::Shutdown()
	{
		// Whatever has not started is dropped, so quitting waits only on the tasks already running.
		{
			std::scoped_lock lock(Mutex);
			Running = false;
			Tasks = {};
		}
		Condition.notify_all();

		for (auto& thread : Workers.Threads)
			if (thread.joinable())
				thread.join();

		Workers.Threads.clear();
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
			// An exception leaving a worker thread would terminate the whole process.
			try
			{
				task();
			}
			catch (const std::exception& e)
			{
				Log::WriteLine(Channel::Error, "A pooled task failed: {}", e.what());
			}
			catch (...)
			{
				Log::WriteLine(Channel::Error, "A pooled task failed.");
			}
		}
	}
}

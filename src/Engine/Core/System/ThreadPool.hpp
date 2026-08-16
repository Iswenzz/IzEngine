#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	struct WorkerThreads
	{
		std::vector<std::thread> Threads;

		~WorkerThreads();
	};

	class API ThreadPool
	{
	public:
		static void Initialize(int threads = std::thread::hardware_concurrency());
		static void Shutdown();
		static void Queue(std::function<void()> task);

	private:
		static void Worker();

		static inline WorkerThreads Workers;
		static inline std::queue<std::function<void()>> Tasks;
		static inline std::mutex Mutex;
		static inline std::condition_variable Condition;
		static inline bool Running = false;
	};
}

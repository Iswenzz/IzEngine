#pragma once

namespace IzEngine
{
	class Crash
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void Sweep();
		static void Patch(uintptr_t base);

	private:
		static inline bool Active = false;

		static void Report(const std::filesystem::path& reporter, const std::filesystem::path& envelope);
	};
}

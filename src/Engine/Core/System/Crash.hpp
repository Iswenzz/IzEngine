#pragma once

namespace IzEngine
{
	class Crash
	{
	public:
		static void Initialize();
		static void Shutdown();

		static bool Available();
		static bool Sending();
		static void Consent(bool allow);

		static void Sweep();
		static void Patch(uintptr_t base);

	private:
		static inline bool Active = false;
		static inline bool Uploads = false;
	};
}

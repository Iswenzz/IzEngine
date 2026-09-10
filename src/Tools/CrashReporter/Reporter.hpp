#pragma once
#include "Report.hpp"

#include "Engine/Common.hpp"

#include <atomic>
#include <mutex>

namespace IzEngine
{
	// The crash window: what happened, a box for what the player was doing, and the Send button.
	class Reporter : public Frame
	{
	public:
		static int Run(CrashReport& report);

		void OnRender() override;

	private:
		CrashReport* Current = nullptr;
		float Scale = 1.0f;
		std::string Description;
		std::string Status;
		bool StatusError = false;
		bool Sending = false;
		bool Failed = false;
		bool Closed = false;

		std::mutex Lock;
		std::string Error;
		std::atomic<int> Outcome = 0;

		Reporter(CrashReport& report, float scale);

		void Content();
		void Details(float height);
		void Buttons();
		void StartSend();
		void Poll();

		static void Fallback(const CrashReport& report);
	};
}

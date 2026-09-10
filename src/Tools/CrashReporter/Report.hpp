#pragma once
#include "Envelope.hpp"

#include <filesystem>
#include <functional>

namespace IzEngine
{
	// Everything the window shows and sends for one crash.
	struct CrashReport
	{
		std::filesystem::path EnvelopePath;
		std::filesystem::path DumpPath;
		std::string Application;
		std::string Version;
		std::string EventId;
		std::vector<std::pair<std::string, std::string>> Fields;
		std::string Details;
		DSN Endpoint;
		Envelope Data;
	};

	class Report
	{
	public:
		static bool Load(const std::filesystem::path& path, CrashReport& report);
		static void Send(CrashReport& report, const std::string& description,
			std::function<void(bool success, const std::string& error)> callback);

	private:
		static void Describe(CrashReport& report, const nlohmann::json& event, const std::string& dump);
		static void Inspect(CrashReport& report, const std::string& dump);
		static const char* ExceptionName(uint32_t code);
	};
}

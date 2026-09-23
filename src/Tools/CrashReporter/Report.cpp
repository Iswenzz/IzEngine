#include "Report.hpp"

#include "Engine/Core/Network/HTTP.hpp"

#include <chrono>
#include <cstring>
#include <format>
#include <fstream>
#include <thread>

namespace IzEngine
{
	constexpr const char* Client = "izengine-crash-reporter/1.0";

	// The minidump layout, as far as the summary needs it. Packed to four bytes like the Windows
	// headers that define it, and read from the buffer field by field, so nothing here depends on
	// the platform the dump came from.
#pragma pack(push, 4)
	struct MinidumpLocation
	{
		uint32_t Size;
		uint32_t Rva;
	};

	struct MinidumpHeader
	{
		uint32_t Signature;
		uint32_t Version;
		uint32_t StreamCount;
		uint32_t StreamDirectoryRva;
		uint32_t CheckSum;
		uint32_t TimeDateStamp;
		uint64_t Flags;
	};

	struct MinidumpDirectory
	{
		uint32_t StreamType;
		MinidumpLocation Location;
	};

	struct MinidumpException
	{
		uint32_t Code;
		uint32_t Flags;
		uint64_t Record;
		uint64_t Address;
		uint32_t ParameterCount;
		uint32_t Alignment;
		uint64_t Information[15];
	};

	struct MinidumpExceptionStream
	{
		uint32_t ThreadId;
		uint32_t Alignment;
		MinidumpException Exception;
		MinidumpLocation ThreadContext;
	};

	struct MinidumpModule
	{
		uint64_t Base;
		uint32_t Size;
		uint32_t CheckSum;
		uint32_t TimeDateStamp;
		uint32_t NameRva;
		uint32_t VersionInfo[13];
		MinidumpLocation CvRecord;
		MinidumpLocation MiscRecord;
		uint64_t Reserved0;
		uint64_t Reserved1;
	};

	struct MinidumpSystemInfo
	{
		uint16_t Architecture;
		uint16_t Level;
		uint16_t Revision;
		uint8_t ProcessorCount;
		uint8_t ProductType;
		uint32_t MajorVersion;
		uint32_t MinorVersion;
		uint32_t BuildNumber;
		uint32_t PlatformId;
		uint32_t CSDVersionRva;
		uint16_t SuiteMask;
		uint16_t Reserved;
	};
#pragma pack(pop)

	constexpr uint32_t MinidumpSignature = 0x504D444D; // "MDMP"
	constexpr uint32_t ModuleListStream = 4;
	constexpr uint32_t ExceptionStream = 6;
	constexpr uint32_t SystemInfoStream = 7;
	constexpr uint16_t ArchitectureAmd64 = 9;
	constexpr uint16_t ArchitectureArm64 = 12;

	template <typename T>
	static bool Read(const std::string& data, size_t offset, T& out)
	{
		if (offset > data.size() || sizeof(T) > data.size() - offset)
			return false;
		std::memcpy(&out, data.data() + offset, sizeof(T));
		return true;
	}

	// Minidump strings are UTF-16LE with a byte length in front.
	static std::string ReadString(const std::string& data, size_t offset)
	{
		uint32_t bytes = 0;
		if (!Read(data, offset, bytes) || bytes > data.size() - offset - sizeof(bytes))
			return {};

		std::string out;
		const size_t start = offset + sizeof(bytes);
		for (size_t i = 0; i + 1 < bytes; i += 2)
		{
			uint32_t code = static_cast<uint8_t>(data[start + i]) | static_cast<uint8_t>(data[start + i + 1]) << 8;
			if (code >= 0xD800 && code <= 0xDBFF && i + 3 < bytes)
			{
				const uint32_t low = static_cast<uint8_t>(data[start + i + 2]) | static_cast<uint8_t>(data[start + i + 3]) << 8;
				if (low >= 0xDC00 && low <= 0xDFFF)
				{
					code = 0x10000 + ((code - 0xD800) << 10) + (low - 0xDC00);
					i += 2;
				}
			}

			if (code < 0x80)
				out += static_cast<char>(code);
			else if (code < 0x800)
				out += { static_cast<char>(0xC0 | code >> 6), static_cast<char>(0x80 | (code & 0x3F)) };
			else if (code < 0x10000)
				out += { static_cast<char>(0xE0 | code >> 12), static_cast<char>(0x80 | (code >> 6 & 0x3F)),
					static_cast<char>(0x80 | (code & 0x3F)) };
			else
				out += { static_cast<char>(0xF0 | code >> 18), static_cast<char>(0x80 | (code >> 12 & 0x3F)),
					static_cast<char>(0x80 | (code >> 6 & 0x3F)), static_cast<char>(0x80 | (code & 0x3F)) };
		}
		return out;
	}

	bool Report::Load(const std::filesystem::path& path, CrashReport& report)
	{
		report.EnvelopePath = path;

		// Crashpad has just closed the file; a scanner that grabbed it gets a moment to let go.
		std::string data;
		for (int attempt = 0; attempt < 20 && data.empty(); attempt++)
		{
			std::ifstream file(path, std::ios::binary);
			if (file)
				data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
			else
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		if (data.empty())
			return false;

		if (!report.Data.Parse(data))
			return false;

		const EnvelopeItem* dump = report.Data.Find("attachment", "event.minidump");
		if (!dump)
			return false;

		report.EventId = report.Data.Headers.value("event_id", "");
		report.Endpoint = Envelope::ParseDSN(report.Data.Headers.value("dsn", ""));

		nlohmann::json event;
		if (const EnvelopeItem* item = report.Data.Find("event"))
			event = nlohmann::json::parse(item->Payload, nullptr, false);
		if (!event.is_object())
			event = nlohmann::json::object();

		// Release is "<app>@<version>", the form sentry gives it.
		const std::string release = event.value("release", "");
		const size_t at = release.find('@');
		report.Application = release.substr(0, at);
		report.Version = at == std::string::npos ? "" : release.substr(at + 1);
		if (report.Application.empty())
			report.Application = "The application";

		// The envelope lives in <reports>/<database>/external; the copy goes to <reports> under the
		// name the tooling reads.
		std::string name = std::filesystem::path(dump->Headers.value("filename", "")).stem().string();
		if (name.empty())
			name = report.EventId.empty() ? "crash" : report.EventId;
		report.DumpPath = path.parent_path().parent_path().parent_path() / (name + "_minidump.dmp");

		std::ofstream out(report.DumpPath, std::ios::binary);
		out.write(dump->Payload.data(), static_cast<std::streamsize>(dump->Payload.size()));
		out.close();

		// Taken away only once the dump copy is safely written, since that copy is what outlives it;
		// it is what keeps a second launch, from the game's sweep or after a killed reporter, from
		// offering the same crash again. A copy that failed leaves the envelope to be offered later.
		if (out)
		{
			std::error_code ec;
			std::filesystem::remove(path, ec);
		}

		Describe(report, event, dump->Payload);
		return true;
	}

	// Answers on the engine's worker thread.
	void Report::Send(CrashReport& report, const std::string& description,
		std::function<void(bool success, const std::string& error)> callback)
	{
		if (!report.Endpoint.Valid)
		{
			callback(false, "This build has nowhere to send reports.");
			return;
		}

		if (!description.empty())
		{
			if (EnvelopeItem* item = report.Data.Find("event"))
			{
				nlohmann::json event = nlohmann::json::parse(item->Payload, nullptr, false);
				if (event.is_object())
				{
					event["extra"]["description"] = description;
					item->Payload = event.dump(-1, ' ', false, nlohmann::json::error_handler_t::replace);
				}
			}
		}

		const auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
		report.Data.Headers["sent_at"] = std::format("{:%FT%T}Z", now);

		HTTPRequest request = HTTP::Post(Envelope::EnvelopeURL(report.Endpoint), report.Data.Serialize(),
			[callback](const HTTPResponse& response)
			{
				if (!response.Success)
				{
					callback(false, response.Error);
					return;
				}
				if (response.Code >= 200 && response.Code < 300)
				{
					callback(true, "");
					return;
				}

				switch (response.Code)
				{
				case 413:
					callback(false, "The report is too large for the server to accept.");
					break;
				case 429:
					callback(false, "The server is rate limiting reports, try again later.");
					break;
				case 401:
				case 403:
					callback(false, std::format("The server refused the report (HTTP {}).", response.Code));
					break;
				default:
					callback(false, std::format("The server answered HTTP {}.", response.Code));
					break;
				}
			});
		request.Headers["X-Sentry-Auth"] = Envelope::AuthHeader(report.Endpoint, Client);
		request.Headers["Content-Type"] = "application/x-sentry-envelope";
		request.Headers["User-Agent"] = Client;
		request.ConnectTimeoutSeconds = 15;
		request.TimeoutSeconds = 0;
		request.LowSpeedLimitBytes = 1024;
		request.LowSpeedTimeSeconds = 60;
		request.Send();
	}

	void Report::Describe(CrashReport& report, const nlohmann::json& event, const std::string& dump)
	{
		const auto line = [&](const char* label, const std::string& value)
		{
			if (!value.empty())
				report.Fields.emplace_back(label, value);
		};

		line("Application", report.Version.empty() ? report.Application : report.Application + " " + report.Version);

		const size_t known = report.Fields.size();
		Inspect(report, dump);

		// Without a readable dump the event's own OS context is the next best thing.
		if (report.Fields.size() == known && event.contains("contexts") && event["contexts"].contains("os"))
		{
			const nlohmann::json& os = event["contexts"]["os"];
			line("System", os.value("name", "") + " " + os.value("version", ""));
		}

		line("Report", report.EventId);
		line("Dump", report.DumpPath.string());

		for (const auto& [label, value] : report.Fields)
			report.Details += std::format("{}: {}\n", label, value);
	}

	void Report::Inspect(CrashReport& report, const std::string& dump)
	{
		const auto line = [&](const char* label, const std::string& value) { report.Fields.emplace_back(label, value); };

		MinidumpHeader header{};
		if (!Read(dump, 0, header) || header.Signature != MinidumpSignature)
			return;

		const auto find = [&](uint32_t type, MinidumpLocation& location)
		{
			for (uint32_t i = 0; i < header.StreamCount; i++)
			{
				MinidumpDirectory directory{};
				if (!Read(dump, header.StreamDirectoryRva + i * sizeof(directory), directory))
					return false;
				if (directory.StreamType == type)
				{
					location = directory.Location;
					return true;
				}
			}
			return false;
		};

		MinidumpLocation location{};
		MinidumpExceptionStream stream{};
		if (find(ExceptionStream, location) && Read(dump, location.Rva, stream))
		{
			const MinidumpException& exception = stream.Exception;
			std::string what = ExceptionName(exception.Code);
			if ((exception.Code == 0xC0000005 || exception.Code == 0xC0000006) && exception.ParameterCount >= 2)
			{
				const char* access = exception.Information[0] == 0 ? "reading"
					: exception.Information[0] == 1                ? "writing"
																   : "executing";
				what += std::format(" {} 0x{:08X}", access, exception.Information[1]);
			}
			line("Exception", std::format("{} (0x{:08X})", what, exception.Code));

			std::string address = std::format("0x{:08X}", exception.Address);
			uint32_t count = 0;
			MinidumpLocation modules{};
			if (find(ModuleListStream, modules) && Read(dump, modules.Rva, count))
			{
				for (uint32_t i = 0; i < count; i++)
				{
					MinidumpModule module{};
					if (!Read(dump, modules.Rva + sizeof(count) + i * sizeof(module), module))
						break;
					if (exception.Address < module.Base || exception.Address >= module.Base + module.Size)
						continue;

					std::string name = ReadString(dump, module.NameRva);
					const size_t separator = name.find_last_of("\\/");
					if (separator != std::string::npos)
						name = name.substr(separator + 1);
					address += std::format("  {}+0x{:X}", name, exception.Address - module.Base);
					break;
				}
			}
			line("Address", address);
			line("Thread", std::format("{}", stream.ThreadId));
		}

		const std::chrono::sys_seconds when{ std::chrono::seconds(header.TimeDateStamp) };
		try
		{
			line("Time", std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::zoned_time(std::chrono::current_zone(), when)));
		}
		catch (const std::exception&)
		{
			line("Time", std::format("{:%Y-%m-%d %H:%M:%S} UTC", when));
		}

		MinidumpSystemInfo system{};
		if (find(SystemInfoStream, location) && Read(dump, location.Rva, system))
		{
			const char* architecture = system.Architecture == ArchitectureAmd64 ? "x64"
				: system.Architecture == ArchitectureArm64                      ? "arm64"
																				: "x86";
			line("System", std::format("Windows {}.{}.{} ({})", system.MajorVersion, system.MinorVersion,
				system.BuildNumber, architecture));
		}
	}

	// NT status codes: what a Windows dump carries, whichever platform reads it.
	const char* Report::ExceptionName(uint32_t code)
	{
		switch (code)
		{
		case 0xC0000005:
			return "Access violation";
		case 0xC0000006:
			return "In-page error";
		case 0xC00000FD:
			return "Stack overflow";
		case 0xC000001D:
			return "Illegal instruction";
		case 0xC0000096:
			return "Privileged instruction";
		case 0xC0000094:
			return "Integer division by zero";
		case 0xC0000095:
			return "Integer overflow";
		case 0xC000008E:
			return "Floating point division by zero";
		case 0xC0000090:
			return "Invalid floating point operation";
		case 0xC0000091:
			return "Floating point overflow";
		case 0xC000008C:
			return "Array bounds exceeded";
		case 0x80000002:
			return "Datatype misalignment";
		case 0xC0000025:
			return "Noncontinuable exception";
		case 0x80000003:
			return "Breakpoint";
		case 0xC0000374:
			return "Heap corruption";
		case 0xC0000409:
			return "Stack buffer overrun";
		case 0xE06D7363:
			return "Unhandled C++ exception";
		// The two codes the engine raises for abort() and std::terminate().
		case 0xE0000001:
			return "Abort";
		case 0xE0000002:
			return "Terminate";
		default:
			return "Unknown exception";
		}
	}
}

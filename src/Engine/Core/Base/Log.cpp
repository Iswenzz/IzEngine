#include "Log.hpp"

#include "Engine/Core/Console/Console.hpp"
#include "Engine/Core/System/System.hpp"

namespace IzEngine
{
	static std::ofstream& File()
	{
		static std::ofstream file = []
		{
			std::ofstream opened;
			if (!System::IsDebug())
				return opened;

			opened.open(APPLICATION_ID ".log", std::ios::app);
			opened << "\nIzEngine started\n" << std::flush;
			return opened;
		}();
		return file;
	}

	static std::string Plain(const std::string& msg)
	{
		std::string out;
		out.reserve(msg.size());

		for (size_t i = 0; i < msg.size(); i++)
		{
			if (msg[i] != '\x1b')
			{
				out.push_back(msg[i]);
				continue;
			}
			while (i < msg.size() && msg[i] != 'm')
				i++;
		}
		return out;
	}

	// One lock over the console too, so threads logging at once do not interleave the Hide and Show
	// that redraw the input line around each message.
	void Log::Write(const std::string& msg)
	{
		static std::mutex guard;
		const std::scoped_lock lock(guard);

		if (File().is_open())
			File() << Plain(msg) << std::flush;

		if (!Console::Handle)
			return;

		Console::Hide();
		const bool eol = msg.find('\n') != std::string::npos;
		const auto endl = eol ? FormatColor(LogColor::Default) : "";
		std::cout << msg << endl;
		Console::Show();
	}
}

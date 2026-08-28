#include "Log.hpp"

#include "Engine/Core/Console/Console.hpp"

namespace IzEngine
{
	static std::ofstream& File()
	{
		static std::ofstream file = []
		{
			std::ofstream opened(APPLICATION_ID ".log", std::ios::app);
			opened << "\n===== session start =====\n" << std::flush;
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

	void Log::Write(const std::string& msg)
	{
		{
			static std::mutex guard;
			const std::scoped_lock lock(guard);

			if (File().is_open())
				File() << Plain(msg) << std::flush;
		}

		if (!Console::Handle)
			return;

		Console::Hide();
		const bool eol = msg.find('\n') != std::string::npos;
		const auto endl = eol ? FormatColor(LogColor::Default) : "";
		std::cout << msg << endl;
		Console::Show();
	}
}

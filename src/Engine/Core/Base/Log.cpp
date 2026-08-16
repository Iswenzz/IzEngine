#include "Log.hpp"

#include "Engine/Core/Console/Console.hpp"

namespace IzEngine
{
	void Log::Write(const std::string& msg)
	{
		if (!Console::Handle)
			return;

		Console::Hide();
		const bool eol = msg.find('\n') != std::string::npos;
		const auto endl = eol ? FormatColor(LogColor::Default) : "";
		std::cout << msg << endl;
		Console::Show();
	}
}

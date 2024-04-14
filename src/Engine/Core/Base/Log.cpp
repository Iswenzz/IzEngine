#include "Log.hpp"

#include "Core/Console/Console.hpp"

namespace IW3SR::Engine
{
	void Log::Write(const std::string& msg)
	{
		Console::Hide();
		const bool eol = msg.find('\n') != std::string::npos;
		const auto endl = eol ? FormatColor(LogColor::Default) : "";
		std::cout << msg << endl;
		Console::Show();
	}
}

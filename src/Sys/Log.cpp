#include "Log.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	void Log::Write(int channel, int type, std::string text)
	{
		std::cout << text;
	}
}

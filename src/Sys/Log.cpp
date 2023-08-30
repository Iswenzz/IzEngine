#include "Log.hpp"
#include <iostream>

namespace IW3SR
{
	Log::Log()
	{
		Com_PrintMessage_h = Hook<Com_PrintMessage_t>(0x4FCA50, Com_PrintMessage);
	}
}

void Com_PrintMessage(int channel, const char* msg, int type)
{
	std::cout << msg;
	Log::Com_PrintMessage_h.Call(channel, msg, type);
}

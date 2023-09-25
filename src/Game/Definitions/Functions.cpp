#include "Functions.hpp"
#include "Game/Game.hpp"

void Com_PrintMessage(int channel, const char* msg, int type)
{
	std::cout << "[A] " << msg;
	SR->Log->Write(channel, type, msg);
	Com_PrintMessage_h.Call(channel, msg, type);
}

Hook<Com_PrintMessage_t> Com_PrintMessage_h(0x4FCA50, Com_PrintMessage);

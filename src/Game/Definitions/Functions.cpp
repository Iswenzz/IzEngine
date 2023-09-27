#include "Functions.hpp"
#include "Game/Game.hpp"

Hook<void(int channel, const char* msg, int type)> Com_PrintMessage_h(0x4FCA50, Com_PrintMessage);

void Com_PrintMessage(int channel, const char* msg, int type)
{
	SR->Log->Write(channel, type, msg);
	Com_PrintMessage_h(channel, msg, type);
}

#include "Functions.hpp"
#include "Game/Game.hpp"

void Com_PrintMessage(int channel, const char* msg, int type)
{
	std::cout << "[A] " << msg;
	SR->Log->Write(channel, type, msg);
	Com_PrintMessage_h(channel, msg, type);
}

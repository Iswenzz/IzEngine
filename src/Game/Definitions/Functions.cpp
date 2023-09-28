#include "Functions.hpp"
#include "Game/Game.hpp"

Hook<void(int channel, const char* msg, int type)> 
	Com_PrintMessage_h(0x4FCA50, Com_PrintMessage);
Hook<void(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)> 
	RB_EndSceneRendering_h(0x658860, RB_EndSceneRendering);

void Com_PrintMessage(int channel, const char* msg, int type)
{
	SR->Log->Write(channel, type, msg);
	Com_PrintMessage_h(channel, msg, type);
}

void RB_EndSceneRendering(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)
{
	RB_EndSceneRendering_h(input, viewInfo, src, buf);
}

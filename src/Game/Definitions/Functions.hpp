#pragma once
#include "Declaration.hpp"
#include "Sys/Hook.hpp"

using namespace IW3SR;

using Com_PrintMessage_t = void(*)(int channel, const char* msg, int type);
using RB_EndSceneRendering_t = void(*)(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf);

void Com_PrintMessage(int channel, const char* msg, int type);
void RB_EndSceneRendering(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf);

extern Hook<Com_PrintMessage_t> Com_PrintMessage_h;
extern Hook<RB_EndSceneRendering_t> RB_EndSceneRendering_h;

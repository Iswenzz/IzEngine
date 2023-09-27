#pragma once
#include "Declaration.hpp"
#include "Sys/Hook.hpp"
#include "Sys/Function.hpp"

using namespace IW3SR;

void Com_PrintMessage(int channel, const char* msg, int type);
void RB_EndSceneRendering(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf);

static Function<dvar_s*(const char* name)> Dvar_FindVar = 0x56B5D0;
extern Hook<void(int channel, const char* msg, int type)> Com_PrintMessage_h;

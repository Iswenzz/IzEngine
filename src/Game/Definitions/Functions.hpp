#pragma once
#include "Declaration.hpp"
#include "Sys/Hook.hpp"
#include "Sys/Function.hpp"

using namespace IW3SR;

void Com_PrintMessage(int channel, const char* msg, int type);
void RB_EndSceneRendering(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf);

static Function<void(int count, int width, GfxPointVertex* verts, bool depthTest)> RB_DrawLines3D = 0x613040;
static Hook<void(int channel, const char* msg, int type)> Com_PrintMessage_h(0x4FCA50, Com_PrintMessage);

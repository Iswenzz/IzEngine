#pragma once
#include "Declaration.hpp"

using Com_PrintMessage_t = void(*)(int channel, const char* msg, int type);

void Com_PrintMessage(int channel, const char* msg, int type);

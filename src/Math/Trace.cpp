#include "Trace.hpp"
#include "Game/Definitions/Functions.hpp"

namespace IW3SR
{
	int Trace::PlayerTrace(trace_t* trace, const vec3_t start, const vec3_t end)
	{
		TraceHitType hitType; //eax
		uint16_t entityNum = 0;

		CG_Trace(trace, start, pmove->mins, pmove->maxs, end, pmove->ps->clientNum, pmove->tracemask);

		if (trace->startsolid && (svs->clients[5].reliableCommandInfo[18].cmd[360] & trace->contents) != 0)
		{
			hitType = trace->hitType;
			switch (hitType)
			{
			case TRACE_HITTYPE_NONE:
				entityNum = 1023;
				break;
			case TRACE_HITTYPE_ENTITY:
				entityNum = trace->hitId;
				break;
			default:
				entityNum = 1022;
			}
			pmove->tracemask &= 0xFDFFFFFF;
			CG_Trace(trace, start, pmove->mins, pmove->maxs, end, pmove->ps->clientNum, pmove->tracemask & 0xFDFFFFFF);
		}
		return 0;
	}
}
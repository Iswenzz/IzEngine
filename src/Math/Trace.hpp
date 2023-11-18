#include "Game/Definitions/Declaration.hpp"

namespace IW3SR
{
	class API Trace
	{
	public:
		static int PlayerTrace(trace_t* trace, const vec3_t start, const vec3_t end);
	};
}
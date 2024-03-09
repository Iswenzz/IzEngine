#include "Sys.hpp"

namespace IW3SR::Engine
{
	bool Sys::IsDebug()
	{
#ifdef _DEBUG
		return true;
#else
		return false;
#endif
	}
}

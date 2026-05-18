#include "System.hpp"

namespace IzEngine
{
	bool System::IsDebug()
	{
#ifdef _DEBUG
		return true;
#else
		return false;
#endif
	}
}

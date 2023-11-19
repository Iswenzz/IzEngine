#include "Utils.hpp"
#include "Macros.hpp"

namespace IW3SR
{
	bool Utils::IsDebug()
	{
		#ifdef NDEBUG
			return true;
		#else
			return false;
		#endif
	}
}

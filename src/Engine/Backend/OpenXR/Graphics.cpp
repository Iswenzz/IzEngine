#include "Graphics.hpp"

namespace IzEngine
{
	// The runtime may submit to the graphics queue inside some of its calls, and a queue another thread
	// submits to as well has to be held for them. One no other thread touches needs nothing.
	void XRGraphics::Lock() { }

	void XRGraphics::Unlock() { }
}

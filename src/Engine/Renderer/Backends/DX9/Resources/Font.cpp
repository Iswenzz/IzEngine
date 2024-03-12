#include "Font.hpp"

namespace IW3SR::Engine
{
	Font::~Font()
	{
		Base->Release();
	}
}

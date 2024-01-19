#include "Font.hpp"

namespace IW3SR
{
	Font::~Font()
	{
		Base->Release();
	}
}

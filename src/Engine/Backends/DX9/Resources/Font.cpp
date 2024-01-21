#include "Font.hpp"

namespace IW3SR::Engine
{
	Font::Font(const std::string& id) : IObject(id) { }

	Font::~Font()
	{
		Base->Release();
	}
}

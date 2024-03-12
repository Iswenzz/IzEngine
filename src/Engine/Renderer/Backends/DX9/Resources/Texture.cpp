#include "Texture.hpp"

namespace IW3SR::Engine
{
	Texture::~Texture()
	{
		Base->Release();
		BaseSurface->Release();
	}
}

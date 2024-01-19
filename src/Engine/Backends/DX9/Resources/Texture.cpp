#include "Texture.hpp"

namespace IW3SR
{
	Texture::~Texture()
	{
		Base->Release();
		BaseSurface->Release();
	}
}

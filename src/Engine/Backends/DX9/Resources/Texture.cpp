#include "Texture.hpp"

namespace IW3SR::Engine
{
	Texture::Texture(const std::string& id) : IObject(id) { }

	Texture::~Texture()
	{
		Base->Release();
		BaseSurface->Release();
	}
}

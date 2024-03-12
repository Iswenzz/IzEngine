#pragma once
#include "DX9/Base.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Texture class.
	/// </summary>
	class API Texture : public IObject
	{
	public:
		IDirect3DTexture9* Base = nullptr;
		IDirect3DSurface9* BaseSurface = nullptr;

		/// <summary>
		/// Create a texture.
		/// </summary>
		Texture() = default;
		~Texture();
	};
}

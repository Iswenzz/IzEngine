#pragma once
#include "Engine/Backends/DX9.hpp"

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
		/// Initialize a new Texture.
		/// </summary>
		/// <param name="id">The font ID.</param>
		Texture(const std::string& id);
		~Texture();
	};
}

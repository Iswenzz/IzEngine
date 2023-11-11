#pragma once
#include <map>
#include <string>
#include <d3dx9.h>

namespace IW3SR
{
	/// <summary>
	/// Assets class.
	/// </summary>
	class Assets
	{
	public:
		static inline std::map<std::string, IDirect3DTexture9*> Images;

		/// <summary>
		/// Initialize the assets.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the assets.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Load image.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		static IDirect3DTexture9* LoadTexture(const std::string& filePath);
	};
}
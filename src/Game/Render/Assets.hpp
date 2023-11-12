#pragma once
#include "Game/Definitions.hpp"

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <d3dx9.h>

namespace IW3SR
{
	/// <summary>
	/// Assets class.
	/// </summary>
	class API Assets
	{
	public:
		static inline std::map<std::string, IDirect3DTexture9*> Images;
		static inline std::unordered_map<std::string, ID3DXFont*> Fonts;
		static inline std::vector<const char*> FontNames;

		/// <summary>
		/// Initialize the assets.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the assets.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Load fonts.
		/// </summary>
		static void LoadFonts();

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		static ID3DXFont* LoadFont(const std::string& name, int height);

		/// <summary>
		/// Load image.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		static IDirect3DTexture9* LoadTexture(const std::string& filePath);
	};
}
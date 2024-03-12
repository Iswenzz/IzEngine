#pragma once
#include "DX9/Base.hpp"

#include "Resources/Texture.hpp"
#include "Resources/Font.hpp"

#include <unordered_map>
#include <vector>

#define FONT_OPENSANS 		"OpenSans-Regular"
#define FONT_SPACERANGER	"Space Ranger"
#define TEXTURE_NULL		"null"

namespace IW3SR::Engine
{
	/// <summary>
	/// Assets class.
	/// </summary>
	class API Assets
	{
		CLASS_SINGLETON(Assets)
	public:
		std::unordered_map<std::string, Ref<Texture>> Textures;
		std::unordered_map<std::string, Ref<Font>> Fonts;
		std::vector<std::string> FontNames;

		/// <summary>
		/// Initialize the assets.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Release the assets.
		/// </summary>
		void Release();

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		Ref<Font> LoadFont(const std::string& name, int height);

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		Ref<Font> LoadFont(const std::filesystem::path& path, int height);

		/// <summary>
		/// Load image.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		Ref<Texture> LoadTexture(const std::filesystem::path& path);

	private:
		/// <summary>
		/// Initialize assets.
		/// </summary>
		Assets() = default;
		virtual ~Assets() = default;

		/// <summary>
		/// Load fonts.
		/// </summary>
		void LoadFonts();
	};
}
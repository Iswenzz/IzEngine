#pragma once
#include "Engine/Backends/DX9.hpp"

#include "Texture.hpp"
#include "Font.hpp"

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

namespace IW3SR
{
	/// <summary>
	/// Assets class.
	/// </summary>
	class API Assets
	{
	public:
		static inline std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
		static inline std::unordered_map<std::string, std::shared_ptr<Font>> Fonts;
		static inline std::vector<std::string> FontNames;

		/// <summary>
		/// Initialize the assets.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the assets.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		static std::shared_ptr<Font> LoadFont(const std::string& name, int height);

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		static std::shared_ptr<Font> LoadFont(const std::filesystem::path& path, int height);

		/// <summary>
		/// Load image.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		static std::shared_ptr<Texture> LoadTexture(const std::string& filePath);

	private:
		/// <summary>
		/// Load fonts.
		/// </summary>
		static void LoadFonts();
	};
}
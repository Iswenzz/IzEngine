#pragma once
#include "Engine/Backends/DX9.hpp"

#include "Resources/Texture.hpp"
#include "Resources/Font.hpp"

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

namespace IW3SR::Engine
{
	/// <summary>
	/// Assets class.
	/// </summary>
	class API Assets : public IInitializable
	{
		CLASS_SINGLETON(Assets)
	public:
		std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
		std::unordered_map<std::string, std::shared_ptr<Font>> Fonts;
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
		std::shared_ptr<Font> LoadFont(const std::string& name, int height);

		/// <summary>
		/// Load a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		std::shared_ptr<Font> LoadFont(const std::filesystem::path& path, int height);

		/// <summary>
		/// Load image.
		/// </summary>
		/// <param name="filePath">The file path.</param>
		std::shared_ptr<Texture> LoadTexture(const std::filesystem::path& path);

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
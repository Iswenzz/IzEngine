#pragma once
#include "Engine/Core/Common.hpp"

#define TEXTURE_BLACK "black"
#define TEXTURE_WHITE "white"

#define SHADER_QUAD "Draw2D_Quad"
#define SHADER_LINE "Draw2D_Line"

#define FONT_OPENSANS "OpenSans-Regular"
#define FONT_SPACERANGER "Space Ranger"

namespace IzEngine
{
	class AssetManager
	{
	public:
		static inline std::unordered_map<std::string, Ref<IResource>> Assets;
		static inline std::vector<std::string> FontNames;

		static void Initialize();
		static void Shutdown();

		static bool Exists(const std::string& id);
		static void Remove(const std::string& id);

		template <typename T>
		static Ref<T> Get(const std::string& id)
		{
			auto it = Assets.find(id);
			if (it == Assets.end())
				return nullptr;
			return std::dynamic_pointer_cast<T>(it->second);
		}

		template <typename T>
		static Ref<T> Add(const std::string& id, const Ref<T>& asset)
		{
			Assets[id] = asset;
			return asset;
		}

	private:
		static void LoadTextures();
		static void LoadFonts();
		static void LoadSystemFonts();
		static void LoadShaders();
	};
}

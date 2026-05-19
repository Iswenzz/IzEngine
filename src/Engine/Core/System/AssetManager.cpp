#include "AssetManager.hpp"
#include "Plugin.hpp"

#include "Engine/Core/IO/VFS.hpp"
#include "Engine/Renderer/Base/Font.hpp"
#include "Engine/Renderer/Base/RendererAPI.hpp"
#include "Engine/Renderer/Base/Shader.hpp"
#include "Engine/Renderer/Base/Texture.hpp"

namespace IzEngine
{
	void AssetManager::Initialize()
	{
		LoadTextures();
		LoadFonts();
		LoadSystemFonts();
		LoadShaders();

		Plugins::Initialize();
	}

	void AssetManager::Shutdown()
	{
		Plugins::Shutdown();
		Assets.clear();
	}

	void AssetManager::LoadTextures()
	{
		Texture::Create({ .ID = TEXTURE_BLACK, .Source = VFS::GetFile("Textures/Engine/black.jpg") });
		Texture::Create({ .ID = TEXTURE_WHITE, .Source = VFS::GetFile("Textures/Engine/white.jpg") });
	}

	void AssetManager::LoadFonts()
	{
		Font::Create({ .ID = FONT_OPENSANS, .Height = 22, .Source = VFS::GetFile("Fonts/OpenSans-Regular.ttf") });
		Font::Create({ .ID = FONT_SPACERANGER, .Height = 22, .Source = VFS::GetFile("Fonts/SpaceRanger.ttf") });
	}

	void AssetManager::LoadShaders()
	{
		Shader::Create({ .ID = SHADER_QUAD,
			.VertexSource = VFS::GetFile(RendererAPI::GetShaderPath("Quad.vertex")),
			.PixelSource = VFS::GetFile(RendererAPI::GetShaderPath("Quad.pixel")) });

		Shader::Create({ .ID = SHADER_LINE,
			.VertexSource = VFS::GetFile(RendererAPI::GetShaderPath("Line.vertex")),
			.PixelSource = VFS::GetFile(RendererAPI::GetShaderPath("Line.pixel")) });
	}

	bool AssetManager::Exists(const std::string& id)
	{
		return Assets.find(id) != Assets.end();
	}

	void AssetManager::Remove(const std::string& id)
	{
		Assets.erase(id);
	}
}

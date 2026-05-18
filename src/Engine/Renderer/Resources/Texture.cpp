#include "Texture.hpp"

#include "Engine/Core/IO/VFS.hpp"
#include "Engine/Core/System/AssetManager.hpp"

#include "Engine/Backend/DX9/Texture.hpp"
#include "Engine/Renderer/Base/RendererAPI.hpp"

namespace IzEngine
{
	Ref<Texture> Texture::Load(const std::string& path)
	{
		return Create({ .Source = VFS::GetFile(path) });
	}

	Ref<Texture> Texture::Create(const TextureSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return DX9Texture::Create(spec);
		}
		IZ_ASSERT(false, "Unknown backend.");
		return nullptr;
	}

	Ref<Texture> Texture::Default()
	{
		auto texture = AssetManager::Get<Texture>(TEXTURE_BLACK);
		IZ_ASSERT(texture, "Default texture not found.");

		if (!texture)
		{
			Log::WriteLine(Channel::Error, "Default texture not found.");
			exit(-1);
		}
		return texture;
	}
}

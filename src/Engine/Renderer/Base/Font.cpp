#include "Font.hpp"

#include "Engine/Core/IO/VFS.hpp"
#include "Engine/Core/System/AssetManager.hpp"

#include "Engine/Backend/DX9/Font.hpp"
#include "Engine/Renderer/Base/RendererAPI.hpp"

namespace IzEngine
{
	Ref<Font> Font::Load(const std::string& path, int size)
	{
		return Create({ .Height = size, .Source = VFS::GetFile(path) });
	}

	Ref<Font> Font::Create(const FontSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererBackend::DX9:
			return DX9Font::Create(spec);
		}
		IZ_ASSERT(false, "Unknown backend.");
		return nullptr;
	}

	Ref<Font> Font::Default()
	{
		auto font = AssetManager::Get<Font>(FONT_OPENSANS);
		IZ_ASSERT(font, "Default font not found.");

		if (!font)
		{
			Log::WriteLine(Channel::Error, "Default font not found.");
			exit(-1);
		}
		return font;
	}
}

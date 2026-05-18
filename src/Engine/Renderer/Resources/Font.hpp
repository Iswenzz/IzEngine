#pragma once
#include "Engine/Core/IO/File.hpp"
#include "Engine/Renderer/Base/GPUResource.hpp"

#define FONT_OPENSANS "OpenSans-Regular"
#define FONT_SPACERANGER "Space Ranger"

namespace IzEngine
{
	struct FontSpecification
	{
		std::string ID;
		int Height = 22;
		int Weight = 400;
		bool Italic = false;
		File Source;
	};

	class API Font : public GPUResource
	{
	public:
		FontSpecification Spec;

		Font() = default;
		virtual ~Font() = default;

		virtual void Release() = 0;
		virtual vec2 GetTextSize(const std::string& text) const = 0;

		static Ref<Font> Load(const std::string& path, int size);
		static Ref<Font> Create(const FontSpecification& spec);
		static Ref<Font> Default();
	};
}

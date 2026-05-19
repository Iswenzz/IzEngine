#pragma once
#include "Engine/Core/IO/File.hpp"
#include "Engine/Renderer/Base/GPUResource.hpp"

namespace IzEngine
{
	enum class TexturePool
	{
		Default,
		Managed,
		SystemMem
	};

	enum class TextureUsage
	{
		None,
		RenderTarget,
		Dynamic
	};

	struct TextureSpecification
	{
		std::string ID;
		vec2 Size;
		int Level = 0;
		TextureUsage Usage = TextureUsage::None;
		TexturePool Pool = TexturePool::Managed;
		File Source;
	};

	class API Texture : public GPUResource
	{
	public:
		TextureSpecification Spec;

		Texture() = default;
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot) const = 0;
		virtual void Unbind(uint32_t slot) const = 0;
		virtual void Release() = 0;

		virtual vec2 GetSize() const = 0;

		static Ref<Texture> Load(const std::string& path);
		static Ref<Texture> Create(const TextureSpecification& spec);
		static Ref<Texture> Default();
	};
}

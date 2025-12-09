#pragma once
#include "Core/Common.hpp"

#define TEXTURE_NULL "null"
#define TEXTURE_WHITE "white"
#define TEXTURE_BLACK "black"

namespace IzEngine
{
	class API Texture : public IObject
	{
	public:
		void* Data = nullptr;
		void* Surface = nullptr;

		Texture() = default;
		virtual ~Texture();

		vec2 GetSize();

		static Ref<Texture>& Create(const std::filesystem::path& path);
		static Ref<Texture>& Create(const std::string& id, const vec2& size);
		static Ref<Texture>& Create(const std::string& id, const vec2& size, int level, int usage, int pool);
		static Ref<Texture>& Default();
	};

	class Textures
	{
	public:
		static inline std::unordered_map<std::string, Ref<Texture>> List;

		static void Initialize();
		static void Shutdown();
	};
}

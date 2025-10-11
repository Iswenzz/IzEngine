#pragma once
#include "Core/Base.hpp"

#define TEXTURE_NULL "null"
#define TEXTURE_WHITE "white"
#define TEXTURE_BLACK "black"

namespace IzEngine
{
	/// <summary>
	/// Texture class.
	/// </summary>
	class API Texture : public IObject
	{
	public:
		void* Data = nullptr;
		void* Surface = nullptr;

		/// <summary>
		/// Create texture.
		/// </summary>
		Texture() = default;
		virtual ~Texture();

		/// <summary>
		/// Create a texture.
		/// </summary>
		/// <param name="path">The file path.</param>
		static Ref<Texture>& Create(const std::filesystem::path& path);

		/// <summary>
		/// Create a texture.
		/// </summary>
		/// <param name="id">The texture id.</param>
		/// <param name="size">The size.</param>
		/// <returns></returns>
		static Ref<Texture>& Create(const std::string& id, const vec2& size);

		/// <summary>
		/// Create a texture.
		/// </summary>
		/// <param name="id">The texture id.</param>
		/// <param name="size">The size.</param>
		/// <param name="level">The level.</param>
		/// <param name="usage">The usage.</param>
		/// <param name="pool">The pool.</param>
		/// <returns></returns>
		static Ref<Texture>& Create(const std::string& id, const vec2& size, int level, int usage, int pool);

		/// <summary>
		/// Get default texture.
		/// </summary>
		/// <returns></returns>
		static Ref<Texture>& Default();
	};

	/// <summary>
	/// Textures class.
	/// </summary>
	class Textures
	{
	public:
		static inline std::unordered_map<std::string, Ref<Texture>> List;

		/// <summary>
		/// Initialize textures.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Release textures.
		/// </summary>
		static void Shutdown();
	};
}

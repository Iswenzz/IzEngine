#pragma once
#include "Core/Base.hpp"

#define FONT_OPENSANS "OpenSans-Regular"
#define FONT_SPACERANGER "Space Ranger"

namespace IW3SR::Engine
{
	/// <summary>
	/// Font class.
	/// </summary>
	class API Font : public IObject
	{
	public:
		void* Data = nullptr;

		/// <summary>
		/// Create font.
		/// </summary>
		Font() = default;
		virtual ~Font();

		/// <summary>
		/// Create a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		static Ref<Font>& Create(const std::string& name, int height);

		/// <summary>
		/// Create a font.
		/// </summary>
		/// <param name="name">The font name.</param>
		/// <param name="height">The font height.</param>
		/// <returns></returns>
		static Ref<Font>& Create(const std::filesystem::path& path, int height);

		/// <summary>
		/// Get default font.
		/// </summary>
		/// <returns></returns>
		static inline Ref<Font>& Default();
	};

	/// <summary>
	/// Fonts class.
	/// </summary>
	class Fonts
	{
	public:
		static inline std::unordered_map<std::string, Ref<Font>> List;
		static inline std::vector<std::string> Names;

		/// <summary>
		/// Initialize fonts.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Release fonts.
		/// </summary>
		static void Release();
	};
}

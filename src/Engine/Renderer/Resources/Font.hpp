#pragma once
#include "Core/Common.hpp"

#include "Core/IO/File.hpp"

#define FONT_OPENSANS "OpenSans-Regular"
#define FONT_SPACERANGER "Space Ranger"

namespace IzEngine
{
	class API Font : public IObject
	{
	public:
		void* Data = nullptr;

		Font() = default;
		virtual ~Font();

		void Release();

		static Ref<Font>& Create(const File& file, int height);
		static Ref<Font>& Create(const std::string& name, int height);
		static Ref<Font>& Default();
	};

	class Fonts
	{
	public:
		static inline std::unordered_map<std::string, Ref<Font>> List;
		static inline std::vector<std::string> Names;

		static void Initialize();
		static void Shutdown();
		static void OnLostDevice();
		static void OnResetDevice();
	};
}

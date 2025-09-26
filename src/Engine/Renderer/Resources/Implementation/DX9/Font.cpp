#include "DX9/Device.hpp"

#include "Core/System/Environment.hpp"
#include "Renderer/Resources/Font.hpp"

namespace IzEngine
{
	Font::~Font()
	{
		if (Data)
			reinterpret_cast<ID3DXFont*>(Data)->Release();
	}

	Ref<Font>& Font::Default()
	{
		return Fonts::List[FONT_OPENSANS];
	}

	Ref<Font>& Font::Create(const std::string& name, int height)
	{
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts::List.find(id); cache != Fonts::List.end())
			return cache->second;

		Ref<Font> font = CreateRef<Font>();
		HRESULT hr =
			D3DXCreateFont(Device::D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), reinterpret_cast<LPD3DXFONT*>(&font->Data));

		if (hr != S_OK)
			return Default();
		return Fonts::List[id] = font;
	}

	Ref<Font>& Font::Create(const std::filesystem::path& path, int height)
	{
		if (!std::filesystem::exists(path))
		{
			Log::WriteLine(Channel::Error, "Font not found: {}", path.string());
			return Default();
		}
		std::string name = path.stem().filename().string();
		std::string id = std::format("{}_{}", name, height);
		if (auto cache = Fonts::List.find(id); cache != Fonts::List.end())
			return cache->second;

		Ref<Font> font = CreateRef<Font>();
		AddFontResource(path.string().c_str());

		HRESULT hr =
			D3DXCreateFont(Device::D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), reinterpret_cast<LPD3DXFONT*>(&font->Data));

		if (hr != S_OK)
			return Default();

		if (std::ranges::find(Fonts::Names, name) == Fonts::Names.end())
			Fonts::Names.push_back(name);

		return Fonts::List[id] = font;
	}

	void Fonts::Initialize()
	{
		IZ_ASSERT(Environment::Initialized, "Environment not initialized.");

		HDC hdc = GetDC(nullptr);
		auto callback = [](const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam)
		{
			Fonts::Names.push_back(lpelf->lfFaceName);
			return TRUE;
		};
		EnumFontFamilies(hdc, nullptr, static_cast<FONTENUMPROC>(callback), 0);
		ReleaseDC(nullptr, hdc);
	}

	void Fonts::Release()
	{
		List.clear();
		Names.clear();
	}
}

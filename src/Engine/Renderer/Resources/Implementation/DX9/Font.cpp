#include "DX9/Common.hpp"

#include "Renderer/Resources/Font.hpp"

#include "Core/IO/VFS.hpp"

namespace IzEngine
{
	Font::~Font()
	{
		Release();
	}

	void Font::Release()
	{
		if (Data)
		{
			reinterpret_cast<ID3DXFont*>(Data)->Release();
			Data = nullptr;
		}
	}

	Ref<Font>& Font::Default()
	{
		IZ_ASSERT(Fonts::List[FONT_OPENSANS], "Default font not found");

		if (!Fonts::List[FONT_OPENSANS])
		{
			Log::WriteLine(Channel::Error, "Default font not found");
			exit(-1);
		}
		return Fonts::List[FONT_OPENSANS];
	}

	Ref<Font>& Font::Create(const File& file, int height)
	{
		std::string name = file.Path.stem().filename().string();
		std::string id = std::format("{}_{}", name, height);

		if (auto cache = Fonts::List.find(id); cache != Fonts::List.end())
			return cache->second;

		if (!file.IsValid())
		{
			Log::WriteLine(Channel::Error, "Font not found: {}", file.Path.string());
			return Default();
		}
		Ref<Font> font = CreateRef<Font>();
		ID3DXFont* dFont = nullptr;

		DWORD count = 0;
		AddFontMemResourceEx(const_cast<uint8_t*>(file.Data.data()), file.Data.size(), nullptr, &count);

		if (FAILED(D3DXCreateFont(Device::D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &dFont)))
			return Default();

		if (std::ranges::find(Fonts::Names, name) == Fonts::Names.end())
			Fonts::Names.push_back(name);

		font->Data = dFont;
		return Fonts::List[id] = font;
	}

	Ref<Font>& Font::Create(const std::string& name, int height)
	{
		std::string id = std::format("{}_{}", name, height);

		if (auto cache = Fonts::List.find(id); cache != Fonts::List.end())
			return cache->second;

		Ref<Font> font = CreateRef<Font>();
		ID3DXFont* dFont = nullptr;

		if (FAILED(D3DXCreateFont(Device::D3Device, height, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &dFont)))
			return Default();

		font->Data = dFont;
		return Fonts::List[id] = font;
	}

	void Fonts::Initialize()
	{
		Font::Create(VFS::GetFile("Fonts/OpenSans-Regular.ttf"), 22);
		Font::Create(VFS::GetFile("Fonts/SpaceRanger.ttf"), 22);

		const auto callback = [](const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam)
		{
			Fonts::Names.push_back(lpelf->lfFaceName);
			return TRUE;
		};
		HDC hdc = GetDC(nullptr);
		EnumFontFamilies(hdc, nullptr, static_cast<FONTENUMPROC>(callback), 0);
		ReleaseDC(nullptr, hdc);
	}

	void Fonts::Shutdown()
	{
		for (auto& [id, font] : List)
			font->Release();

		List.clear();
		Names.clear();
	}

	void Fonts::OnLostDevice()
	{
		for (auto& [id, font] : List)
			reinterpret_cast<ID3DXFont*>(font->Data)->OnLostDevice();
	}

	void Fonts::OnResetDevice()
	{
		for (auto& [id, font] : List)
			reinterpret_cast<ID3DXFont*>(font->Data)->OnResetDevice();
	}
}

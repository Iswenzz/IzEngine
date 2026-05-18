#include "Font.hpp"
#include "GraphicsContext.hpp"

#include "Engine/Core/System/AssetManager.hpp"

namespace IzEngine
{
	Ref<Font> DX9Font::Create(const FontSpecification& spec)
	{
		std::string name = !spec.ID.empty() ? spec.ID : spec.Source.Path.string();
		std::string id = std::format("{}_{}", name, spec.Height);

		IZ_ASSERT(!name.empty(), "FontSpecification requires an ID or a Source.");

		if (AssetManager::Exists(id))
			return AssetManager::Get<Font>(id);

		if (spec.Source.IsValid())
		{
			DWORD count = 0;
			AddFontMemResourceEx(const_cast<uint8_t*>(spec.Source.Data.data()), spec.Source.Data.size(), nullptr,
				&count);
		}
		Ref<DX9Font> font = CreateRef<DX9Font>();
		ID3DXFont* dFont = nullptr;

		if (FAILED(D3DXCreateFont(DX9GraphicsContext::Device, spec.Height, 0, spec.Weight, 1,
				spec.Italic ? TRUE : FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &dFont)))
		{
			Log::WriteLine(Channel::Error, "Font not found: {}", name);
			return Font::Default();
		}
		if (std::ranges::find(AssetManager::FontNames, name) == AssetManager::FontNames.end())
			AssetManager::FontNames.push_back(name);

		font->Data = dFont;
		font->Spec = spec;

		GPUResource::RegisterResource(font.get());
		return AssetManager::Add(id, font);
	}

	DX9Font::~DX9Font()
	{
		GPUResource::UnregisterResource(this);
		Release();
	}

	void DX9Font::Release()
	{
		if (Data)
		{
			Data->Release();
			Data = nullptr;
		}
	}

	void DX9Font::OnDeviceLost()
	{
		if (Data)
			Data->OnLostDevice();
	}

	void DX9Font::OnDeviceReset()
	{
		if (Data)
		{
			Data->OnResetDevice();
			return;
		}
		std::string name = !Spec.ID.empty() ? Spec.ID : Spec.Source.Path.string();
		ID3DXFont* dFont = nullptr;

		if (FAILED(D3DXCreateFont(DX9GraphicsContext::Device, Spec.Height, 0, Spec.Weight, 1,
				Spec.Italic ? TRUE : FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &dFont)))
		{
			Log::WriteLine(Channel::Error, "Failed to recreate font after device reset: {}", name);
			return;
		}
		Data = dFont;
	}

	vec2 DX9Font::GetTextSize(const std::string& text) const
	{
		IZ_ASSERT(Data, "Font data is null.");
		RECT rect = { 0 };
		Data->DrawTextA(nullptr, text.c_str(), -1, &rect, DT_CALCRECT, 0);
		return { static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top) };
	}
}

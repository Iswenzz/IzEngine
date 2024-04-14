#include "DX9/Device.hpp"

#include "Renderer/Drawing/Draw2D.hpp"

namespace IW3SR::Engine
{
	void Draw2D::Text(const std::string& text, const Ref<Font>& font, const vec2& position, const vec2& size,
		const vec4& color)
	{
		auto data = reinterpret_cast<ID3DXFont*>(font->Data);
		RECT rect = { static_cast<int>(position.x), static_cast<int>(position.y), 0, 0 };
		data->DrawTextA(nullptr, text.c_str(), -1, &rect, DT_NOCLIP, color.BGRA());
	}

	vec2 Draw2D::TextSize(const std::string& text, const Ref<Font>& font)
	{
		RECT rect = { 0 };
		auto data = reinterpret_cast<ID3DXFont*>(font->Data);
		data->DrawTextA(nullptr, text.c_str(), -1, &rect, DT_CALCRECT, 0);
		return { static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top) };
	}

	void Draw2D::Rect(const Ref<Texture>& texture, const vec2& position, const vec2& size)
	{
		auto surface = reinterpret_cast<IDirect3DSurface9*>(texture->Surface);
		RECT rect = { static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(size.x),
			static_cast<int>(size.y) };
		Device::D3Device->StretchRect(surface, nullptr, nullptr, &rect, D3DTEXF_NONE);
	}
}

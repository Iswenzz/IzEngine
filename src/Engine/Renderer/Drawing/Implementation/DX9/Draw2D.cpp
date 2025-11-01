#include "DX9/Device.hpp"

#include "Renderer/Drawing/Draw2D.hpp"

namespace IzEngine
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
		DWORD oldFVF;
		Device::D3Device->GetFVF(&oldFVF);
		 
		auto tex = reinterpret_cast<IDirect3DTexture9*>(texture->Data);
		Device::D3Device->SetTexture(0, tex);
		Device::D3Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

		// clang-format off
		float vertices[24] = {
			position.x, position.y, 0.0f, 1.0f, 0.0f, 0.0f,
			position.x + size.x, position.y, 0.0f, 1.0f, 1.0f, 0.0f,
			position.x + size.x, position.y + size.y, 0.0f, 1.0f, 1.0f, 1.0f,
			position.x, position.y + size.y, 0.0f, 1.0f, 0.0f, 1.0f
		};
		// clang-format on
		Device::D3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, 6 * sizeof(float));
		Device::D3Device->SetFVF(oldFVF);
	}
}

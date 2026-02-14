#include "DX9/Common.hpp"

#include "Renderer/Drawing/Draw2D.hpp"

namespace IzEngine
{
	void Draw2D::Text(const std::string& text, const Ref<Font>& font, const vec2& position, const vec2& size,
		const vec4& color)
	{
		auto data = reinterpret_cast<ID3DXFont*>(font->Data);
		RECT rect = { static_cast<int>(position.x), static_cast<int>(position.y), 0, 0 };
		data->DrawTextA(nullptr, text.c_str(), -1, &rect, DT_NOCLIP, Math::BGRA(color));
	}

	void Draw2D::Text(const std::string& text, const Ref<Font>& font, const vec2& position, const vec2& size,
		const vec2& skew, const vec4& color)
	{
		static ID3DXSprite* sprite = nullptr;
		if (!sprite)
			D3DXCreateSprite(Device::D3Device, &sprite);

		auto data = reinterpret_cast<ID3DXFont*>(font->Data);
		RECT rect = { static_cast<int>(position.x), static_cast<int>(position.y), 0, 0 };

		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);
		matrix._21 = skew.x;
		matrix._12 = skew.y;

		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->SetTransform(&matrix);
		data->DrawTextA(sprite, text.c_str(), -1, &rect, DT_NOCLIP, Math::BGRA(color));
		sprite->End();
	}

	vec2 Draw2D::TextSize(const std::string& text, const Ref<Font>& font)
	{
		RECT rect = { 0 };
		auto data = reinterpret_cast<ID3DXFont*>(font->Data);
		data->DrawTextA(nullptr, text.c_str(), -1, &rect, DT_CALCRECT, 0);
		return { static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top) };
	}

	void Draw2D::Rect(const Ref<Texture>& texture, const vec2& position, const vec2& size, const vec4& color)
	{
		DWORD oldFVF;
		Device::D3Device->GetFVF(&oldFVF);

		auto tex = reinterpret_cast<IDirect3DTexture9*>(texture->Data);
		Device::D3Device->SetTexture(0, tex);
		Device::D3Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		DWORD col = Math::RGBA(color);

		struct Vertex
		{
			float x, y, z, rhw;
			DWORD color;
			float u, v;
		};
		Vertex vertices[4] = { { position.x, position.y, 0.0f, 1.0f, col, 0.0f, 0.0f },
			{ position.x + size.x, position.y, 0.0f, 1.0f, col, 1.0f, 0.0f },
			{ position.x + size.x, position.y + size.y, 0.0f, 1.0f, col, 1.0f, 1.0f },
			{ position.x, position.y + size.y, 0.0f, 1.0f, col, 0.0f, 1.0f } };

		Device::D3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(Vertex));
		Device::D3Device->SetFVF(oldFVF);
	}
}

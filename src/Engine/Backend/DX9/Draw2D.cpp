#include "Base.hpp"
#include "Font.hpp"
#include "GraphicsContext.hpp"

#include "Engine/Renderer/Drawing/Draw2D.hpp"

namespace IzEngine
{
	void Draw2D::DrawText(const std::string& text, const Ref<Font>& font, const vec3& position, float size,
		const vec4& color, const vec2& skew)
	{
		auto dxFont = std::static_pointer_cast<DX9Font>(font);
		IZ_ASSERT(dxFont->Data, "Font data is null.");

		if (skew.x != 0.0f || skew.y != 0.0f)
		{
			static ID3DXSprite* sprite = nullptr;
			if (!sprite)
				D3DXCreateSprite(DX9GraphicsContext::Device, &sprite);

			D3DXMATRIX matrix;
			D3DXMatrixIdentity(&matrix);
			matrix._21 = skew.x;
			matrix._12 = skew.y;

			RECT rect = { static_cast<int>(position.x), static_cast<int>(position.y), 0, 0 };

			sprite->Begin(D3DXSPRITE_ALPHABLEND);
			sprite->SetTransform(&matrix);
			dxFont->Data->DrawTextA(sprite, text.c_str(), -1, &rect, DT_NOCLIP, Math::BGRA(color));
			sprite->End();
		}
		else
		{
			RECT rect = { static_cast<int>(position.x), static_cast<int>(position.y), 0, 0 };
			dxFont->Data->DrawTextA(nullptr, text.c_str(), -1, &rect, DT_NOCLIP, Math::BGRA(color));
		}
	}
}

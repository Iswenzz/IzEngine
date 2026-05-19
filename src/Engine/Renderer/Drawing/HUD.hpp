#pragma once
#include "Engine/Core/Common.hpp"

#include "Engine/Renderer/Base/Texture.hpp"

namespace IzEngine
{
	class API HUD : public IObject
	{
	public:
		vec2 Position;
		vec2 Size;
		vec2 RenderPosition;
		vec2 RenderSize;
		vec4 Color;

		Horizontal HorizontalAlign = Horizontal::Left;
		Vertical VerticalAlign = Vertical::Top;
		Alignment AlignX = Alignment::Left;
		Alignment AlignY = Alignment::Top;
		std::string TextureName;

		HUD() = default;
		HUD(const std::string& texture, float x, float y, float w, float h, const vec4& color);
		virtual ~HUD() = default;

		void SetRectAlignment(Horizontal horizontal, Vertical vertical);
		void SetAlignment(Alignment horizontal, Alignment vertical);
		void SetTexture(const std::string& texture);

		void Menu(const std::string& label, bool open = false);
		void Render();

	private:
		Ref<Texture> Texture = nullptr;

		void ComputeAlignment(vec2& position);

		SERIALIZE_POLY_BASE(HUD, Position, Size, Color, HorizontalAlign, VerticalAlign, AlignX, AlignY, TextureName)
	};
}

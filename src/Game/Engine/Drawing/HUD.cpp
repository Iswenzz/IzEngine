#include "HUD.hpp"
#include "Draw2D.hpp"

#include "Engine/Backends/DX9/Device.hpp"
#include "Engine/Backends/ImGUI/Components.hpp"

namespace IW3SR::Game
{
	HUD::HUD(const std::string& material, float x, float y, float w, float h, const vec4& color)
	{
		Position = { x, y };
		Size = { w, h };
		Color = color;
		MaterialName = material;
	}

	void HUD::SetRectAlignment(Horizontal horizontal, Vertical vertical)
	{
		HorizontalAlign = horizontal;
		VerticalAlign = vertical;
	}

	void HUD::SetAlignment(Alignment horizontal, Alignment vertical)
	{
		AlignX = horizontal;
		AlignY = vertical;
	}

	void HUD::SetMaterial(const std::string& material)
	{
		Material = Material_RegisterHandle(material.c_str(), 3);
		MaterialName = material;
	}

	void HUD::ComputeAlignment(float& x, float& y)
	{
		if (AlignX == ALIGN_CENTER)
			x += -(Size.x / 2.f);
		else if (AlignX == ALIGN_RIGHT)
			x += -Size.x;

		if (AlignY == ALIGN_MIDDLE)
			y += Size.y / 2.f;
		else if (AlignY == ALIGN_BOTTOM)
			y += Size.y;
	}

	void HUD::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label.c_str(), open ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
			return;

		ImGui::PushID(ID.c_str());

		const std::vector<std::string>& horizontals = Draw2D::HorizontalAlignment;
		const std::vector<std::string>& verticals = Draw2D::VerticalAlignment;
		const std::vector<std::string>& fonts = Assets::Get().FontNames;

		ImGui::DragFloat2("Position", Position);
		ImGui::DragFloat2("Size", Size);
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

		int horizontal = HorizontalAlign - 1;
		if (ImGui::Combo("Horizontal Alignment", &horizontal, horizontals))
			HorizontalAlign = static_cast<Horizontal>(horizontal + 1);

		int vertical = VerticalAlign - 1;
		if (ImGui::Combo("Vertical Alignment", &vertical, verticals))
			VerticalAlign = static_cast<Vertical>(vertical + 1);

		int alignX = AlignX / 4;
		if (ImGui::Combo("Align X", &alignX, horizontals))
			AlignX = static_cast<Alignment>(alignX * 4);

		int alignY = AlignY;
		if (ImGui::Combo("Align Y", &alignY, verticals))
			AlignY = static_cast<Alignment>(alignY);

		ImGui::PopID();
	}

	void HUD::Render()
	{
		float x = Position.x;
		float y = Position.y;
		float w = Size.x;
		float h = Size.y;

		if (!Material)
			SetMaterial(MaterialName);

		ComputeAlignment(x, y);
		Device::Get().Screen.Apply(x, y, w, h, HorizontalAlign, VerticalAlign);
		R_AddCmdDrawStretchPic(Material, x, y, w, h, 0.f, 0.f, 0.f, 0.f, Color);
	}
}

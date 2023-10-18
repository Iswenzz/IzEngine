#include "ImGUI.hpp"
#include "imgui_internal.h"

namespace ImGui
{
	bool ButtonId(const std::string& label, const std::string& id, const ImVec2& size)
	{
		ImGui::PushID(id.c_str());
		bool state = ImGui::Button(label.c_str(), size);
		ImGui::PopID();
		return state;
	}

    void ToggleButton(const std::string& id, float size, bool* v)
    {
        ImVec2 p = GetCursorScreenPos();
        ImDrawList* draw_list = GetWindowDrawList();

        float width = size * 1.55f;
        float radius = size * 0.50f;

        InvisibleButton(id.c_str(), ImVec2(width, size));

        if (IsItemClicked())
            *v = !*v;

        float t = *v ? 1.0f : 0.0f;

        ImGuiContext& g = *GImGui;
        float ANIM_SPEED = 0.08f;

        if (g.LastActiveId == g.CurrentWindow->GetID(id.c_str()))
        {
            float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
            t = *v ? (t_anim) : (1.0f - t_anim);
        }

        ImU32 col_bg;

        if (IsItemHovered())
            col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
        else
            col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + size), col_bg, size * 0.5f);
        draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
    }
}

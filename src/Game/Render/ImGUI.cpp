#include "ImGUI.hpp"
#include "Game/Render/GUI.hpp"

namespace ImGui
{
	bool ButtonId(const std::string& label, const std::string& id, bool* v, const ImVec2& size)
	{
		PushID(id.c_str());
		Button(label.c_str(), size);
		PopID();

        bool clicked = ImGui::IsItemClicked();
        if (clicked) *v = !*v;
		return clicked;
	}

    bool ToggleButton(const std::string& id, float size, bool* v)
    {
        ImVec2 p = GetCursorScreenPos();
        ImDrawList* draw = GetWindowDrawList();
        ImGuiContext& g = *GImGui;

        float width = size * 1.55f;
        float radius = size * 0.50f;

        InvisibleButton(id.c_str(), ImVec2(width, size));
        bool clicked = ImGui::IsItemClicked();
        if (clicked) *v = !*v;

        float t = *v ? 1.0f : 0.0f;
        const float ANIM_SPEED = 0.08f;

        if (g.LastActiveId == g.CurrentWindow->GetID(id.c_str()))
        {
            float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
            t = *v ? (t_anim) : (1.0f - t_anim);
        }
        ImU32 color = IsItemHovered()
            ? GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t))
            : GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

        draw->AddRectFilled(p, ImVec2(p.x + width, p.y + size), color, size * 0.5f);
        draw->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), 
            radius - 1.5f, IM_COL32(255, 255, 255, 255));

        return clicked;
    }

    void BeginGroupBox(const std::string& name, const ImVec2& size, const ImColor& color)
    {
        ImDrawList* draw = GetWindowDrawList();
        ImGuiWindow* window = GetCurrentWindow();
        ImVec2 pos = window->DC.CursorPos;

        BeginChild(std::string(name).append(".main").c_str(), size, false, ImGuiWindowFlags_NoScrollbar);

        draw->AddRectFilled(pos + ImVec2{ 0, 20 }, pos + size, color, 6);
        draw->AddText(pos + ImVec2{ 12, -5 }, GetColorU32(ImGuiCol_Text, 1), name.c_str());

        SetCursorPos(ImVec2(12, 21));
        PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 10 });
        BeginChild(name.c_str(), {size.x - 24, size.y - 21}, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

        BeginGroup();

        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 8, 10 });
    }

    void EndGroupBox()
    {
        PopStyleVar(2);
        EndGroup();
        EndChild();
        EndChild();
    }
}

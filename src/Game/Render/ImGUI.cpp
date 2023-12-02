#include "ImGUI.hpp"
#include "GUI.hpp"

namespace ImGui
{
    bool Button(const std::string& label, bool* v, const ImVec2& size)
    {
        Button(label.c_str(), size);

        bool clicked = IsItemClicked();
        if (v && clicked) *v = !*v;
        return clicked;
    }

	bool Button(const std::string& label, const std::string& id, bool* v, const ImVec2& size)
	{
		PushID(id.c_str());
		Button(label.c_str(), size);
		PopID();

        bool clicked = IsItemClicked();
        if (v && clicked) *v = !*v;
		return clicked;
	}

    bool ButtonToggle(const std::string& label, const std::string& id, bool* v, const ImVec2& size)
    {
        if (*v) 
        {
            PushStyleColor(ImGuiCol_Button, GetStyleColorVec4(ImGuiCol_ButtonActive));
            PushStyleColor(ImGuiCol_ButtonHovered, GetStyleColorVec4(ImGuiCol_ButtonActive));
        }
        PushID(id.c_str());
        Button(label.c_str(), size);
        PopID();

        if (*v) PopStyleColor(2);

        bool clicked = IsItemClicked();
        if (clicked) *v = !*v;
        return clicked;
    }

    bool Toggle(const std::string& id, float size, bool* v)
    {
        ImVec2 p = GetCursorScreenPos();
        ImDrawList* draw = GetWindowDrawList();
        ImGuiContext* g = GImGui;

        float width = size * 1.55f;
        float radius = size * 0.50f;

        InvisibleButton(id.c_str(), ImVec2(width, size));
        bool clicked = IsItemClicked();
        if (clicked) *v = !*v;

        float t = *v ? 1.0f : 0.0f;
        const float ANIM_SPEED = 0.08f;

        if (g->LastActiveId == g->CurrentWindow->GetID(id.c_str()))
        {
            float t_anim = ImSaturate(g->LastActiveIdTimer / ANIM_SPEED);
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

    bool Combo(const std::string& label, int* item, const std::vector<std::string>& items, int maxHeightInItems)
    {
        std::vector<const char*> list;
        for (const auto& item : items)
            list.push_back(item.c_str());

        return Combo(label.c_str(), item, list.data(), list.size(), maxHeightInItems);
    }

    void Tooltip(const std::string& text)
    {
        if (!IsItemHovered())
            return;

        ImVec2 size = GetItemRectSize();
        ImVec2 position = GetItemRectMin();
        position.y += size.y + 2;

        SetNextWindowPos(position);
        PushStyleVar(ImGuiStyleVar_WindowPadding, { 6, 6 });

        BeginTooltip();
        Text(text.c_str());
        EndTooltip();

        PopStyleVar();
    }

    void Keybind(const std::string& label, int* key, const ImVec2& size)
    {
        char keyLabel[100];
        const int keyLabelSize = 100;
        const auto id = GetID(label.c_str());

        ImGui::KeepAliveID(id);
        PushID(id);

        DWORD code = MapVirtualKey(*key, MAPVK_VK_TO_VSC);
        GetKeyNameText(code << 16, keyLabel, keyLabelSize);
        CharUpperBuff(keyLabel, keyLabelSize);

        if (GetActiveID() == id) 
        {
            int k = 0;
            ImGuiIO& io = ImGui::GetIO();

            PushStyleColor(ImGuiCol_Button, GetColorU32(ImGuiCol_ButtonActive));
            Button("...", size);
            PopStyleColor();

            for (; k < ImGuiKey_COUNT; k++)
            {
                if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(k)))
                {
                    *key = k;
                    break;
                }
            }
            if (k != ImGuiKey_COUNT || (!IsItemHovered() && IsMouseClicked(ImGuiMouseButton_Left)))
                ClearActiveID();
        }
        else if (Button(keyLabel, size))
            SetActiveID(id, GetCurrentWindow());

        SameLine();
        Text(label.c_str());

        PopID();
    }

    void Movable(const std::string& label, vec2& position, vec2& size, vec2& renderPosition, vec2& renderSize)
    {
        if (!GUI::Open || !GUI::DesignMode) return;

        SetNextWindowBgAlpha(0.2f);
        PushStyleColor(ImGuiCol_Border, { 0.2, 0.2, 0.2, 0.2 });
        Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar);

        if (IsWindowHovered() && IsMouseDragging(ImGuiMouseButton_Left))
        {
            renderPosition = GetWindowPos();
            renderSize = GetWindowSize();
           
            position += vec2(scr_place->scaleRealToVirtual) * vec2(GetMouseDragDelta());
            size = vec2(scr_place->scaleRealToVirtual) * renderSize;
            ResetMouseDragDelta();
        }
        else
        {
            SetWindowPos(renderPosition);
            SetWindowSize(renderSize);
        }
        End();
        PopStyleColor();
    }

    void Rainbow(const vec2& position, const vec2& size)
    {
        ImDrawList* draw = ImGui::GetForegroundDrawList();

        const auto& [rainbow1, rainbow2] = GUI::Themes.Rainbow;
        draw->AddRectFilledMultiColor(position, size, rainbow1, rainbow2, rainbow2, rainbow1);
    }
}

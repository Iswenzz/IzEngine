#include "Components.hpp"
#include "UI.hpp"

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

    bool Toggle(const std::string& id, bool* v, const ImVec2& defaultSize)
    {
        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        ImVec2 p = GetCursorScreenPos();
        ImDrawList* draw = GetWindowDrawList();

        float fontSize = g.FontSize;
        float height = fontSize + style.FramePadding.y / 2.0f;
        float width = height + style.FramePadding.x * 2.5f;
        float radius = height * 0.5f;
        ImVec2 size = CalcItemSize(defaultSize, width, height);

        InvisibleButton(id.c_str(), size);
        bool clicked = IsItemClicked();
        if (clicked) *v = !*v;

        const float ANIM_SPEED = 0.08f;
        float t = *v ? 1.0f : 0.0f;

        if (g.LastActiveId == g.CurrentWindow->GetID(id.c_str()))
        {
            float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
            t = *v ? (t_anim) : (1.0f - t_anim);
        }
        const ImU32 color = IsItemHovered()
            ? GetColorU32(ImLerp({ 0.78f, 0.78f, 0.78f, 1.0f }, { 0.64f, 0.83f, 0.34f, 1.0f }, t))
            : GetColorU32(ImLerp({ 0.85f, 0.85f, 0.85f, 1.0f }, { 0.56f, 0.83f, 0.26f, 1.0f }, t));
        const ImU32 white = GetColorU32({ 1, 1, 1, 1 });

        draw->AddRectFilled(p, { p.x + size.x, p.y + size.y }, color, size.y * 0.5f);
        draw->AddCircleFilled({ p.x + radius + t * (size.x - radius * 2.0f), p.y + radius }, radius - 1.5f, white);

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

        KeepAliveID(id);
        PushID(id);

        DWORD code = MapVirtualKey(*key, MAPVK_VK_TO_VSC);
        GetKeyNameText(code << 16, keyLabel, keyLabelSize);
        CharUpperBuff(keyLabel, keyLabelSize);

        if (GetActiveID() == id)
        {
            PushStyleColor(ImGuiCol_Button, GetColorU32(ImGuiCol_ButtonActive));
            Button("...", size);
            PopStyleColor();

            int k = 0;
            for (; k < ImGuiKey_COUNT; k++)
            {
                if (KeyListener::IsPressed(k))
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
        if (!UI::Get().Open || !UI::Get().DesignMode) return;

        PushStyleColor(ImGuiCol_Border, { 0.2, 0.2, 0.2, 0.2 });
        SetNextWindowBgAlpha(0.2f);
        SetNextWindowPos(renderPosition, ImGuiCond_FirstUseEver);
        SetNextWindowSize(renderSize, ImGuiCond_FirstUseEver);
        Begin(label.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

        vec2 framePosition = GetWindowPos();
        vec2 frameSize = GetWindowSize();
        vec2 dragDelta = GetMouseDragDelta();

        if (IsMouseDragging(ImGuiMouseButton_Left) && dragDelta)
        {
            if (IsWindowHovered() || IsWindowResizing())
            {
                position += UI::Get().Screen.RealToVirtual * (framePosition - renderPosition);
                size += UI::Get().Screen.RealToVirtual * (frameSize - renderSize);

                renderPosition = framePosition;
                renderSize = frameSize;
                ResetMouseDragDelta();
            }
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
        ImDrawList* draw = GetForegroundDrawList();

        const auto& [rainbow1, rainbow2] = UI::Get().Themes.Rainbow;
        draw->AddRectFilledMultiColor(position, size, rainbow1, rainbow2, rainbow2, rainbow1);
    }

    void Markdown(const std::string& markdown)
    {
        Markdown(markdown.c_str(), markdown.size(), UI::Get().Themes.Markdown);
    }

    bool IsWindowResizing()
    {
        if (!IsMouseDragging(ImGuiMouseButton_Left))
            return false;

        ImGuiWindow *window = GetCurrentWindow();
        ImGuiID active = GetActiveID();

        for (int i = 0; i < 4; i++)
        {
            if (GetWindowResizeCornerID(window, i) == active)
                return true;
            if (GetWindowResizeBorderID(window, i) == active)
                return true;
        }
        return false;
    }
}

#include "ESP.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
    ESP::ESP()
    {
        ID = "sr.player.esp";
        Name = "ESP";

        Color = { 0, 1, 1, 1 };
        Box = { 12, 12, 65 };
        Size = 1;
    }

    void ESP::OnMenu()
    {
        ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);
    }

    void ESP::OnFrame()
    {
        ImDrawList* draw = ImGui::GetBackgroundDrawList();
        const vec2 center = { cgs->refdef.width / 2.f, cgs->refdef.height * 1.f };
        const int limit = center.y;

        for (const auto& player : Player::GetAll())
        {
            if (!player || player->IsSelf() || !player->IsAlive())
                continue;

            const vec3 self = cgs->lastVieworg;
            const vec3 origin = player->c->pose.origin;

            if (self.Distance(origin) < 50)
                continue;

            const vec2 position = WorldToScreen(origin);
            if (!position || position.y > limit)
                continue;

            draw->AddLine(center, position, Color, Size);
            Draw::Box(origin, Box, Color, Size);
        }
    }
}

#include "Draw3D.hpp"

namespace IW3SR
{
    void Draw3D::Box(const vec3& position, const vec3& size, const vec4& color, int thickness)
    {
        ImDrawList* draw = ImGui::GetBackgroundDrawList();

        vec3 tfl3 = { position.x + size.x, position.y - size.y, position.z + size.z };
        vec3 tfr3 = { position.x + size.x, position.y + size.y, position.z + size.z };
        vec3 tbl3 = { position.x - size.x, position.y + size.y, position.z + size.z };
        vec3 tbr3 = { position.x - size.x, position.y - size.y, position.z + size.z };

        vec2 tfl = Math::WorldToScreen(tfl3);
        vec2 tfr = Math::WorldToScreen(tfr3);
        vec2 tbl = Math::WorldToScreen(tbl3);
        vec2 tbr = Math::WorldToScreen(tbr3);

        vec3 bfl3 = { position.x + size.x, position.y - size.y, position.z };
        vec3 bfr3 = { position.x + size.x, position.y + size.y, position.z };
        vec3 bbl3 = { position.x - size.x, position.y + size.y, position.z };
        vec3 bbr3 = { position.x - size.x, position.y - size.y, position.z };

        vec2 bfl = Math::WorldToScreen(bfl3);
        vec2 bfr = Math::WorldToScreen(bfr3);
        vec2 bbl = Math::WorldToScreen(bbl3);
        vec2 bbr = Math::WorldToScreen(bbr3);

        draw->AddLine(tbl, tfr, color, thickness);
        draw->AddLine(tfl, tfr, color, thickness);
        draw->AddLine(tfl, tbr, color, thickness);
        draw->AddLine(tbl, tbr, color, thickness);

        draw->AddLine(bbl, bfr, color, thickness);
        draw->AddLine(bfr, bfl, color, thickness);
        draw->AddLine(bfl, bbr, color, thickness);
        draw->AddLine(bbr, bbl, color, thickness);

        draw->AddLine(bbr, tbr, color, thickness);
        draw->AddLine(bbl, tbl, color, thickness);
        draw->AddLine(bfr, tfr, color, thickness);
        draw->AddLine(bfl, tfl, color, thickness);
    }
}

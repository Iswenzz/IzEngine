#include "ESP.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
    ESP::ESP()
    {
        ID = "sr.player.esp";
        Name = "ESP";

        Color = { 1, 0, 0, 1 };
        Lines = std::make_unique<class Lines>(2, 64, false);
    }

    void ESP::OnFrame()
    {
        const vec3 eye = { 0, 0, 60 };
        const vec3 forward = AngleToForward(cgs->refdefViewAngles) * 45.f;
        const vec3 position = vec3{ clients->cgameOrigin } + eye;
        const vec3 start = forward + position;

        for (const auto& player : Player::GetAll())
        {
            if (player->IsSelf())
                continue;

            const vec3 end = player->c->pose.origin;
            Lines->Add(start, end, Color);
        }
        Lines->Frame();
    }
}

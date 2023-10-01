#include "ESP.hpp"
#include "Game/Render/Draw.hpp"

#include <iostream>

namespace IW3SR
{
    ESP::ESP()
    {
        ID = "sr.player.esp";
        Name = "ESP";
    }

    void ESP::OnFrame()
    {
        const int height = 30;
        const int radius = 40;
        const glm::vec3 origin = glm::to_vec3(cgs->lastVieworg);
        const glm::vec3 center = glm::vec3(origin.x, origin.y, height + origin.z);
        const glm::vec4 color{ 1, 0, 0, 1 };

        glm::vec3 start = origin - glm::vec3(radius, 0.0f, -height);
        glm::vec3 end = origin + glm::vec3(radius, 0.0f, height);
        Draw::Line(start, end, color);

        start = origin - glm::vec3(0.0f, radius, -height);
        end = origin + glm::vec3(0.0f, radius, height);
        Draw::Line(start, end, color);
    }
}

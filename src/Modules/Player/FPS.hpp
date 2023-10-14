#pragma once
#include "Modules/Module.hpp"
#include "Game/Render/Draw/Text.hpp"

namespace IW3SR
{
    /// <summary>
    /// Draw current FPS.
    /// </summary>
    class FPS : public Module
    {
    public:
        Text FPSText;
        vec4 Color;
        vec2 Pos;

        /// <summary>
        /// Initialize the module.
        /// </summary>
        FPS();
        virtual ~FPS() = default;

        /// <summary>
        /// Draw 2D.
        /// </summary>
        void OnDraw2D() override;
    };
}

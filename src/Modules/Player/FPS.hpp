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
        vec2 Position;

        /// <summary>
        /// Initialize the module.
        /// </summary>
        FPS();
        virtual ~FPS() = default;

        /// <summary>
        /// Menu drawing.
        /// </summary>
        void OnMenu() override;

        /// <summary>
        /// Draw 2D.
        /// </summary>
        void OnDraw2D() override;

        NLOHMANN_DEFINE_POLY(FPS, Module, FPSText, Color, Position);
    };
}

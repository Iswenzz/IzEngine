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
        /// Menu drawing.
        /// </summary>
        void OnMenu() override;

        /// <summary>
        /// Draw 2D.
        /// </summary>
        void OnDraw2D() override;

    private:
        static inline std::vector<const char*> fonts = { FONT_OBJECTIVE, FONT_NORMAL, FONT_CONSOLE,
            FONT_SMALL, FONT_SMALL_DEV, FONT_BIG, FONT_BIG_DEV, FONT_BOLD };

        static inline int currentFont = 0;
    };
}

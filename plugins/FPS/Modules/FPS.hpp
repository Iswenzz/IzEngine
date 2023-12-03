#pragma once
#include "Game/Modules.hpp"

namespace IW3SR
{
    /// <summary>
    /// Draw current FPS.
    /// </summary>
    class FPS : public Module
    {
    public:
        Text FPSText;

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
        /// Render frame.
        /// </summary>
        void OnFrame() override;

        NLOHMANN_DEFINE_POLY(FPS, Module, FPSText)
    };
}

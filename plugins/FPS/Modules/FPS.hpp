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
        std::vector<int> Values;
        bool ShowPlot;

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

        NLOHMANN_SERIALIZE_POLY(FPS, Module, FPSText)
    };
}

#pragma once
#include "Game/Plugin.hpp"
#include "Engine/Backends/ImGUI/Drawing/Notification.hpp"

namespace IW3SR::Addons
{
    /// <summary>
    /// Draw current FPS.
    /// </summary>
    class FPS : public Module
    {
    public:
        int Value;
        CircularBuffer<int, 1000> Values;
        Text FrameText;

        Plots Graph;
        bool ShowGraph;

        /// <summary>
        /// Create the module.
        /// </summary>
        FPS();
        virtual ~FPS() = default;

        /// <summary>
        /// Initialize the module.
        /// </summary>
        void Initialize() override;

        /// <summary>
        /// Menu drawing.
        /// </summary>
        void OnMenu() override;

        /// <summary>
        /// Render frame.
        /// </summary>
        void OnRender() override;

        NLOHMANN_SERIALIZE_POLY(FPS, Module, FrameText, Graph, ShowGraph)
    };
}

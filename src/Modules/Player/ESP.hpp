#pragma once
#include "Modules/Module.hpp"
#include "Game/Render/Draw.hpp"

namespace IW3SR
{
    /// <summary>
    /// Extra Sensory Perception.
    /// </summary>
    class ESP : public Module
    {
    public:
        vec4 Color;
        vec3 Box;
        int Size;

        /// <summary>
        /// Initialize the module.
        /// </summary>
        ESP();
        virtual ~ESP() = default;

        /// <summary>
        /// Draw 2D.
        /// </summary>
        void OnDraw2D() override;

        /// <summary>
        /// Frame callback.
        /// </summary>
        void OnFrame() override;
    };
}

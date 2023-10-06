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
        std::unique_ptr<class Lines> Lines;

        /// <summary>
        /// Initialize the module.
        /// </summary>
        ESP();
        virtual ~ESP() = default;

        /// <summary>
        /// Draw callback.
        /// </summary>
        void OnFrame() override;
    };
}

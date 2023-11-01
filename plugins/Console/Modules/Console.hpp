#pragma once
#include "Modules/Module.hpp"

namespace IW3SR
{
    /// <summary>
    /// Display an external console.
    /// </summary>
    class Console : public Module
    {
    public:
        /// <summary>
        /// Initialize the module.
        /// </summary>
        Console();
        virtual ~Console() = default;

        /// <summary>
        /// Initialize the console.
        /// </summary>
        void Initialize() override;

        /// <summary>
        /// Shutdown the console.
        /// </summary>
        void Shutdown() override;
    };
}

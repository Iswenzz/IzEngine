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
        /// Initialize the console.
        /// </summary>
        Console();
        ~Console();

        /// <summary>
        /// Initialize the module.
        /// </summary>
        void Initialize() override;

        /// <summary>
        /// Shutdown the module.
        /// </summary>
        void Shutdown() override;
    };
}

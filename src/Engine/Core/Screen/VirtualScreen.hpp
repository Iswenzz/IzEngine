#pragma once
#include "Engine/Core/Math.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Virtual screen.
	/// </summary>
	class VirtualScreen
	{
	public:
		vec2 Position;
		vec2 Size;
        vec2 DisplaySize;
        float AdjustedRealWidth = 0;

        vec2 RealMin;
        vec2 RealMax;
        vec2 VirtualMin;
        vec2 VirtualMax;
        vec2 VirtualToReal;
        vec2 VirtualToFull;
        vec2 RealToVirtual;
        vec2 SubScreen;

        /// <summary>
        /// Initialize the virtual screen.
        /// </summary>
        VirtualScreen() = default;

        /// <summary>
        /// Initialize the virtual screen.
        /// </summary>
        /// <param name="position">The position.</param>
        /// <param name="size">The size.</param>
        /// <param name="display">The display size.</param>
        VirtualScreen(const vec2& position, const vec2& size, const vec2& display);
        ~VirtualScreen() = default;

        /// <summary>
        /// Setup the virtual screen.
        /// </summary>
        void Setup();

        /// <summary>
        /// Compute the safe area.
        /// </summary>
        void SafeArea();

        /// <summary>
        /// Virtual to real screen.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        /// <param name="horizontal">Horizontal aligment.</param>
        /// <param name="vertical">Vertical aligment.</param>
        void Apply(float& x, float& y, Horizontal horizontal, Vertical vertical);

        /// <summary>
        /// Virtual to real screen.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        /// <param name="w">The witdth.</param>
        /// <param name="h">The height.</param>
        /// <param name="horizontal">Horizontal aligment.</param>
        /// <param name="vertical">Vertical aligment.</param>
        void Apply(float& x, float& y, float& w, float& h, Horizontal horizontal, Vertical vertical);

        /// <summary>
        /// Real to virtual screen.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        /// <param name="horizontal">Horizontal aligment.</param>
        /// <param name="vertical">Vertical aligment.</param>
        void Reverse(float& x, float& y, Horizontal horizontal, Vertical vertical);

        /// <summary>
        /// Real to virtual screen.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        /// <param name="w">The witdth.</param>
        /// <param name="h">The height.</param>
        /// <param name="horizontal">Horizontal aligment.</param>
        /// <param name="vertical">Vertical aligment.</param>
        void Reverse(float& x, float& y, float& w, float& h, Horizontal horizontal, Vertical vertical);
	};
}

#pragma once
#include "Core/Math.hpp"

namespace IzEngine
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
		/// <param name="size">The size.</param>
		VirtualScreen(const vec2& size);
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
		/// <param name="position">The position.</param>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void Apply(vec2& position, Horizontal horizontal, Vertical vertical);

		/// <summary>
		/// Virtual to real screen.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void Apply(vec2& position, vec2& size, Horizontal horizontal, Vertical vertical);

		/// <summary>
		/// Real to virtual screen.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void Reverse(vec2& position, Horizontal horizontal, Vertical vertical);

		/// <summary>
		/// Real to virtual screen.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void Reverse(vec2& position, vec2& size, Horizontal horizontal, Vertical vertical);
	};
}

#pragma once
#include "Game/Definitions.hpp"
#include "Engine/Core/Math.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// World class.
	/// </summary>
	class API World
	{
		CLASS_SINGLETON(World);
	public:
		/// <summary>
		/// Converts a 3D position to screen space.
		/// </summary>
		/// <param name="position">The 3D position.</param>
		/// <returns></returns>
		static vec2 ToScreen(const vec3& position);

	private:
		/// <summary>
		/// Initialize world.
		/// </summary>
		World() = default;
		~World() = default;
	};
}

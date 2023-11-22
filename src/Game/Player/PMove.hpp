#pragma once
#include "Game/Definitions.hpp"
#include "Math/Math.hpp"

namespace IW3SR
{
	/// <summary>
	/// Player movement.
	/// </summary>
	class API PMove
	{
	public:
		/// <summary>
		/// Finish moving.
		/// </summary>
		/// <param name="cmd">The user command.</param>
		static void FinishMove(usercmd_s* cmd);

		/// <summary>
        /// Set yaw.
        /// </summary>
        /// <param name="cmd">The user command.</param>
        /// <param name="angles">The angles.</param>
        /// <param name="target">The target.</param>
        static void SetYaw(usercmd_s* cmd, float* angles, const vec3& target);

        /// <summary>
        /// Set pitch.
        /// </summary>
        /// <param name="cmd">The user command.</param>
        /// <param name="angles">The angles.</param>
        /// <param name="target">The target.</param>
        static void SetPitch(usercmd_s* cmd, float* angles, const vec3& target);
	};
}
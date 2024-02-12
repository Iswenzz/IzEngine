#pragma once
#include "Player.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Player movement.
	/// </summary>
	/// <remarks>PMove is shared with self and spectator client.</remarks>
	class API PMove
	{
	public:
		/// <summary>
		/// Finish moving.
		/// </summary>
		/// <param name="cmd">The user command.</param>
		static void FinishMove(usercmd_s* cmd);

		/// <summary>
		/// Walk moving.
		/// </summary>
		/// <param name="pm">The player movement.</param>
		/// <param name="pml">The player movement library.</param>
		static void WalkMove(pmove_t* pm, pml_t* pml);

		/// <summary>
		/// Air moving.
		/// </summary>
		/// <param name="pm">The player movement.</param>
		/// <param name="pml">The player movement library.</param>
		static void AirMove(pmove_t* pm, pml_t* pml);

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

		/// <summary>
		/// Set yaw and pitch.
		/// </summary>
		/// <param name="cmd">The user command.</param>
		/// <param name="angles">The angles.</param>
		/// <param name="target">The target.</param>
		static void SetAngles(usercmd_s* cmd, float* angles, const vec3& target);

		/// <summary>
		/// Is player on ground.
		/// </summary>
		/// <returns></returns>
		static bool OnGround();

		/// <summary>
		/// Is player in air.
		/// </summary>
		/// <returns></returns>
		static bool InAir();
	};
}
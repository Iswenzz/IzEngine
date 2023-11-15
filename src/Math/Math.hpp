#pragma once
// https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_shared.h
// https://github.com/xoxor4d/iw3xo-dev/blob/master/src/utils/vector.hpp
#include "Game/Definitions.hpp"

#include "Matrix3.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

#define M_PI	3.14159265358979323846f

#define	PITCH	0
#define	YAW		1
#define	ROLL	2

namespace IW3SR
{
    /// <summary>
    /// Math class.
    /// </summary>
    class API Math
    {
    public:
        /// <summary>
        /// Converts a 3D world position to screen coordinates.
        /// </summary>
        /// <param name="worldPosition">World position in 3D space.</param>
        /// <returns>Screen coordinates as a 2D vector.</returns>
        static vec2 WorldToScreen(const vec3& worldPosition);

        /// <summary>
        /// Converts radians to degrees.
        /// </summary>
        /// <param name="radians">Radians value.</param>
        /// <returns>Degrees value.</returns>
        static float RadToDeg(const float radians);

        /// <summary>
        /// Converts degrees to radians.
        /// </summary>
        /// <param name="degrees">Degrees value.</param>
        /// <returns>Radians value.</returns>
        static float DegToRad(const float degrees);

        /// <summary>
        /// Normalizes an angle to the range of -180 to 180 degrees.
        /// </summary>
        /// <param name="angle">Angle in degrees.</param>
        /// <returns>Normalized angle in degrees.</returns>
        static float AngleNormalize180(const float angle);

        /// <summary>
        /// Normalizes an angle to the range of -PI to PI radians.
        /// </summary>
        /// <param name="angle">Angle in radians.</param>
        /// <returns>Normalized angle in radians.</returns>
        static float AngleNormalizePi(const float angle);

        /// <summary>
        /// Converts Euler angles to a forward, right, and up vector.
        /// </summary>
        /// <param name="angles">Euler angles in radians.</param>
        /// <param name="forward">Output forward vector.</param>
        /// <param name="right">Output right vector.</param>
        /// <param name="up">Output up vector.</param>
        static void AngleVectors(const vec3& angles, vec3& forward, vec3& right, vec3& up);

        /// <summary>
        /// Converts Euler angles to a up vector.
        /// </summary>
        /// <param name="angles">Euler angles in radians.</param>
        /// <returns>Up vector.</returns>
        static vec3 AnglesToUp(const vec3& angles);

        /// <summary>
        /// Converts Euler angles to a forward vector.
        /// </summary>
        /// <param name="angles">Euler angles in radians.</param>
        /// <returns>Forward vector.</returns>
        static vec3 AnglesToForward(const vec3& angles);

        /// <summary>
        /// Converts Euler angles to a right vector.
        /// </summary>
        /// <param name="angles">Euler angles in radians.</param>
        /// <returns>Right vector.</returns>
        static vec3 AnglesToRight(const vec3& angles);

        /// <summary>
        /// Converts a 3D vector to Euler angles (pitch, yaw, and roll) in radians.
        /// </summary>
        /// <param name="v">Input vector.</param>
        /// <returns>Euler angles in radians.</returns>
        static vec3 VectorToAngles(const vec3& v);

        /// <summary>
        /// Converts a forward and up vector to Euler angles (pitch, yaw, and roll) in radians.
        /// </summary>
        /// <param name="forward">Forward vector.</param>
        /// <param name="up">Up vector.</param>
        /// <param name="flipPitch">True to flip the pitch angle.</param>
        static vec3 VectorToAnglesWithRoll(const vec3& forward, const vec3& up, bool flipPitch);

        /// <summary>
        /// Checks if an angle is within the field of view (FOV).
        /// </summary>
        /// <param name="angle">Angle to check in radians.</param>
        /// <returns>True if the angle is within the FOV, false otherwise.</returns>
        static bool AngleInFov(float angle);

        /// <summary>
        /// Projects an angle onto the screen for rendering.
        /// </summary>
        /// <param name="angle">Angle to project in radians.</param>
        /// <returns>Screen projection of the angle.</returns>
        static float AngleScreenProjection(float angle);

        /// <summary>
        /// Converts a range specified by start and end angles to a range structure based on yaw.
        /// </summary>
        /// <param name="start">Starting angle in radians.</param>
        /// <param name="end">Ending angle in radians.</param>
        /// <param name="yaw">Yaw angle in radians.</param>
        /// <returns>Range structure representing the angles based on yaw.</returns>
        static range_t AnglesToRange(float start, float end, float yaw);

        /// <summary>
        /// Virtual screen to real screen with rect aligment.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        /// <param name="horizontal">Horizontal aligment.</param>
        /// <param name="vertical">Vertical aligment.</param>
        static void ApplyRect(float& x, float& y, RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical);

        /// <summary>
        /// Virtual screen to real screen with rect aligment.
        /// </summary>
        /// <param name="x">X value.</param>
        /// <param name="y">Y value.</param>
        /// <param name="w">The witdth.</param>
        /// <param name="h">The height.</param>
        /// <param name="horizontal">Horizontal aligment.</param>
        /// <param name="vertical">Vertical aligment.</param>
        static void ApplyRect(float& x, float& y, float& w, float& h,
            RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical);
    };
}

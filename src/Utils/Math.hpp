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
    /// Converts a 3D world position to screen coordinates.
    /// </summary>
    /// <param name="worldPosition">World position in 3D space.</param>
    /// <returns>Screen coordinates as a 2D vector.</returns>
    vec2 WorldToScreen(const vec3& worldPosition);

    /// <summary>
    /// Converts radians to degrees.
    /// </summary>
    /// <param name="radians">Radians value.</param>
    /// <returns>Degrees value.</returns>
    float RadToDeg(const float radians);

    /// <summary>
    /// Converts degrees to radians.
    /// </summary>
    /// <param name="degrees">Degrees value.</param>
    /// <returns>Radians value.</returns>
    float DegToRad(const float degrees);

    /// <summary>
    /// Normalizes an angle to the range of -180 to 180 degrees.
    /// </summary>
    /// <param name="angle">Angle in degrees.</param>
    /// <returns>Normalized angle in degrees.</returns>
    float AngleNormalize180(const float angle);

    /// <summary>
    /// Normalizes an angle to the range of -PI to PI radians.
    /// </summary>
    /// <param name="angle">Angle in radians.</param>
    /// <returns>Normalized angle in radians.</returns>
    float AngleNormalizePi(const float angle);

    /// <summary>
    /// Converts Euler angles to a forward, right, and up vector.
    /// </summary>
    /// <param name="angles">Euler angles in radians.</param>
    /// <param name="forward">Output forward vector.</param>
    /// <param name="right">Output right vector.</param>
    /// <param name="up">Output up vector.</param>
    void AngleVectors(const vec3& angles, vec3& forward, vec3& right, vec3& up);

    /// <summary>
    /// Converts Euler angles to a forward vector.
    /// </summary>
    /// <param name="angles">Euler angles in radians.</param>
    /// <returns>Forward vector.</returns>
    vec3 AngleToForward(const vec3& angles);

    /// <summary>
    /// Converts Euler angles to a right vector.
    /// </summary>
    /// <param name="angles">Euler angles in radians.</param>
    /// <returns>Right vector.</returns>
    vec3 AngleToRight(const vec3& angles);

    /// <summary>
    /// Converts a 3D vector to Euler angles (pitch, yaw, and roll) in radians.
    /// </summary>
    /// <param name="v">Input vector.</param>
    /// <returns>Euler angles in radians.</returns>
    vec3 VectorToAngles(const vec3& v);

    /// <summary>
    /// Converts a forward and up vector to Euler angles (pitch, yaw, and roll) in radians.
    /// </summary>
    /// <param name="forward">Forward vector.</param>
    /// <param name="up">Up vector.</param>
    /// <param name="flipPitch">True to flip the pitch angle.</param>
    vec3 VectorToAnglesWithRoll(const vec3& forward, const vec3& up, bool flipPitch);

    /// <summary>
    /// Adjusts the coordinates and size from a 640x480 resolution to the current resolution.
    /// </summary>
    /// <param name="x">Pointer to the X-coordinate.</param>
    /// <param name="y">Pointer to the Y-coordinate.</param>
    /// <param name="w">Pointer to the width.</param>
    /// <param name="h">Pointer to the height.</param>
    void AdjustFrom640(float& x, float& y, float& w, float& h);

    /// <summary>
    /// Fills a rectangle with the specified color.
    /// </summary>
    /// <param name="x">X-coordinate of the top-left corner.</param>
    /// <param name="y">Y-coordinate of the top-left corner.</param>
    /// <param name="w">Width of the rectangle.</param>
    /// <param name="h">Height of the rectangle.</param>
    /// <param name="color">The rect color.</param>
    void FillRect(float x, float y, float w, float h, const vec4& color);

    /// <summary>
    /// Fills a section of a circle with a specified yaw angle, starting and ending angles, and color.
    /// </summary>
    /// <param name="start">Starting angle in radians.</param>
    /// <param name="end">Ending angle in radians.</param>
    /// <param name="yaw">Yaw angle in radians.</param>
    /// <param name="y">Y-coordinate of the circle's center.</param>
    /// <param name="h">Radius of the circle.</param>
    /// <param name="color">The color.</param>
    void FillAngleYaw(float start, float end, float yaw, float y, float h, const vec4& color);

    /// <summary>
    /// Draws a line at a specified yaw angle with a given width, height, and color.
    /// </summary>
    /// <param name="angle">Angle at which to draw the line in radians.</param>
    /// <param name="yaw">Yaw angle in radians.</param>
    /// <param name="y">Y-coordinate of the line's position.</param>
    /// <param name="w">Width of the line.</param>
    /// <param name="h">Height of the line.</param>
    /// <param name="color">The color.</param>
    void DrawLineYaw(float angle, float yaw, float y, float w, float h, const vec4& color);

    /// <summary>
    /// Checks if an angle is within the field of view (FOV).
    /// </summary>
    /// <param name="angle">Angle to check in radians.</param>
    /// <returns>True if the angle is within the FOV, false otherwise.</returns>
    bool AngleInFov(float angle);

    /// <summary>
    /// Projects an angle onto the screen for rendering.
    /// </summary>
    /// <param name="angle">Angle to project in radians.</param>
    /// <returns>Screen projection of the angle.</returns>
    float AngleScreenProjection(float angle);

    /// <summary>
    /// Converts a range specified by start and end angles to a range structure based on yaw.
    /// </summary>
    /// <param name="start">Starting angle in radians.</param>
    /// <param name="end">Ending angle in radians.</param>
    /// <param name="yaw">Yaw angle in radians.</param>
    /// <returns>Range structure representing the angles based on yaw.</returns>
    range_t AnglesToRange(float start, float end, float yaw);
}

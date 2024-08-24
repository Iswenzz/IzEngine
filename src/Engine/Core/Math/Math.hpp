#pragma once
// https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_shared.h
// https://github.com/xoxor4d/iw3xo-dev/blob/master/src/utils/vector.hpp
#include "Engine/Base.hpp"

#define M_PI 3.14159265358979323846f

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define PITCH 0
#define YAW 1
#define ROLL 2

#define ANGLE2SHORT(x) (static_cast<int>((x) * 65536 / 360) & 65535)
#define SHORT2ANGLE(x) (x * (360.0f / 65536))

#define DotProduct2(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1])
#define DotProduct3(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define DotProduct4(x, y) ((x)[0] * (y)[0] + (x)[1] * (y)[1] + (x)[2] * (y)[2] + (x)[3] * (y)[3])

#define CrossProduct2(a, b, c) ((c) = (a)[0] * (b)[1] - (a)[1] * (b)[0])
#define CrossProduct3(a, b, c)                                                               \
	((c)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1], (c)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2], \
		(c)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0])
#define CrossProduct4(a, b, c)                                                               \
	((c)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1], (c)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2], \
		(c)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0], (c)[3] = 0)

#define VectorAdd2(a, b, c) ((c)[0] = (a)[0] + (b)[0], (c)[1] = (a)[1] + (b)[1])
#define VectorAdd3(a, b, c) ((c)[0] = (a)[0] + (b)[0], (c)[1] = (a)[1] + (b)[1], (c)[2] = (a)[2] + (b)[2])
#define VectorAdd4(a, b, c) \
	((c)[0] = (a)[0] + (b)[0], (c)[1] = (a)[1] + (b)[1], (c)[2] = (a)[2] + (b)[2], (c)[3] = (a)[3] + (b)[3])

#define VectorSubtract2(a, b, c) ((c)[0] = (a)[0] - (b)[0], (c)[1] = (a)[1] - (b)[1])
#define VectorSubtract3(a, b, c) ((c)[0] = (a)[0] - (b)[0], (c)[1] = (a)[1] - (b)[1], (c)[2] = (a)[2] - (b)[2])
#define VectorSubtract4(a, b, c) \
	((c)[0] = (a)[0] - (b)[0], (c)[1] = (a)[1] - (b)[1], (c)[2] = (a)[2] - (b)[2], (c)[3] = (a)[3] - (b)[3])

#define VectorCopy2(a, b) ((b)[0] = (a)[0], (b)[1] = (a)[1])
#define VectorCopy3(a, b) ((b)[0] = (a)[0], (b)[1] = (a)[1], (b)[2] = (a)[2])
#define VectorCopy4(a, b) ((b)[0] = (a)[0], (b)[1] = (a)[1], (b)[2] = (a)[2], (b)[3] = (a)[3])

#define VectorScale2(v, s, o) ((o)[0] = (v)[0] * (s), (o)[1] = (v)[1] * (s))
#define VectorScale3(v, s, o) ((o)[0] = (v)[0] * (s), (o)[1] = (v)[1] * (s), (o)[2] = (v)[2] * (s))
#define VectorScale4(v, s, o) \
	((o)[0] = (v)[0] * (s), (o)[1] = (v)[1] * (s), (o)[2] = (v)[2] * (s), (o)[3] = (v)[3] * (s))

#define VectorMultiplyAdd2(v, s, b, o) ((o)[0] = (v)[0] + (b)[0] * (s), (o)[1] = (v)[1] + (b)[1] * (s))
#define VectorMultiplyAdd3(v, s, b, o) \
	((o)[0] = (v)[0] + (b)[0] * (s), (o)[1] = (v)[1] + (b)[1] * (s), (o)[2] = (v)[2] + (b)[2] * (s))
#define VectorMultiplyAdd4(v, s, b, o)                                                               \
	((o)[0] = (v)[0] + (b)[0] * (s), (o)[1] = (v)[1] + (b)[1] * (s), (o)[2] = (v)[2] + (b)[2] * (s), \
		(o)[3] = (v)[3] + (b)[3] * (s))

#define VectorLengthSquared2(v) ((v)[0] * (v)[0] + (v)[1] * (v)[1])
#define VectorLengthSquared3(v) ((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2])
#define VectorLengthSquared4(v) ((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2] + (v)[3] * (v)[3])

#define VectorLength2(v) (sqrt(VectorLengthSquared2(v)))
#define VectorLength3(v) (sqrt(VectorLengthSquared3(v)))
#define VectorLength4(v) (sqrt(VectorLengthSquared4(v)))

#define VectorNegate2(a, b) ((b)[0] = -(a)[0], (b)[1] = -(a)[1])
#define VectorNegate3(a, b) ((b)[0] = -(a)[0], (b)[1] = -(a)[1], (b)[2] = -(a)[2])
#define VectorNegate4(a, b) ((b)[0] = -(a)[0], (b)[1] = -(a)[1], (b)[2] = -(a)[2], (b)[3] = -(a)[3])

#define VectorClear2(a) ((a)[0] = (a)[1] = 0)
#define VectorClear3(a) ((a)[0] = (a)[1] = (a)[2] = 0)
#define VectorClear4(a) ((a)[0] = (a)[1] = (a)[2] = (a)[3] = 0)

#define VectorSet2(v, x, y) ((v)[0] = (x), (v)[1] = (y))
#define VectorSet3(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))
#define VectorSet4(v, x, y, z, w) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z), v[3] = (w))

namespace IzEngine
{
	template <typename T>
	class Vector2;
	template <typename T>
	class Vector3;
	template <typename T>
	class Vector4;
	template <typename T>
	class Matrix3;

	using vec2 = Vector2<float>;
	using vec3 = Vector3<float>;
	using vec4 = Vector4<float>;
	using mat3 = Matrix3<float>;

	enum Horizontal
	{
		HORIZONTAL_SUBLEFT,		   // Left edge of a 4:3 screen (safe area not included).
		HORIZONTAL_LEFT,		   // Left viewable (safe area) edge.
		HORIZONTAL_CENTER,		   // Center of the screen (reticle).
		HORIZONTAL_RIGHT,		   // Right viewable (safe area) edge.
		HORIZONTAL_FULLSCREEN,	   // Disregards safe area.
		HORIZONTAL_NOSCALE,		   // Uses exact parameters.
		HORIZONTAL_TO640,		   // Scales a real-screen resolution x down into the 0 - 640 range.
		HORIZONTAL_CENTER_SAFEAREA // Center of the safearea.
	};

	enum Vertical
	{
		VERTICAL_SUBTOP,		 // Top edge of the 4:3 screen (safe area not included).
		VERTICAL_TOP,			 // Top viewable (safe area) edge.
		VERTICAL_CENTER,		 // Center of the screen (reticle).
		VERTICAL_BOTTOM,		 // Bottom viewable (safe area) edge.
		VERTICAL_FULLSCREEN,	 // Disregards safe area.
		VERTICAL_NOSCALE,		 // Uses exact parameters.
		VERTICAL_TO480,			 // Scales a real-screen resolution y down into the 0 - 480 range.
		VERTICAL_CENTER_SAFEAREA // Center of the save area.
	};

	enum Alignment
	{
		ALIGN_TOP = 0,
		ALIGN_LEFT = 0,
		ALIGN_MIDDLE = 1,
		ALIGN_BOTTOM = 2,
		ALIGN_CENTER = 4,
		ALIGN_RIGHT = 8
	};

	/// <summary>
	/// Math class.
	/// </summary>
	class API Math
	{
	public:
		/// <summary>
		/// Converts radians to degrees.
		/// </summary>
		/// <param name="radians">Radians value.</param>
		/// <returns>Degrees value.</returns>
		static float RadToDeg(float radians);

		/// <summary>
		/// Converts degrees to radians.
		/// </summary>
		/// <param name="degrees">Degrees value.</param>
		/// <returns>Radians value.</returns>
		static float DegToRad(float degrees);

		/// <summary>
		/// Normalizes an angle to the range of 0 to 90 degrees.
		/// </summary>
		/// <param name="angle">Angle in degrees.</param>
		/// <returns></returns>
		static float AngleNormalize90(float angle);

		/// <summary>
		/// Normalizes an angle to the range of -180 to 180 degrees.
		/// </summary>
		/// <param name="angle">Angle in degrees.</param>
		/// <returns></returns>
		static float AngleNormalize180(float angle);

		/// <summary>
		/// Normalizes an angle to the range of 0 to 360 degrees.
		/// </summary>
		/// <param name="angle">Angle in degrees.</param>
		/// <returns></returns>
		static float AngleNormalize360(float angle);

		/// <summary>
		/// Normalizes an angle to the range of -PI to PI radians.
		/// </summary>
		/// <param name="angle">Angle in radians.</param>
		/// <returns>Normalized angle in radians.</returns>
		static float AngleNormalizePI(float angle);

		/// <summary>
		/// Gets the delta angle of 2 angles.
		/// </summary>
		/// <param name="angle1">The first angle.</param>
		/// <param name="angle2">The second angle.</param>
		/// <returns></returns>
		static float AngleDelta(float angle1, float angle2);

		/// <summary>
		/// Checks if an angle is within the field of view (FOV).
		/// </summary>
		/// <param name="angle">Angle to check in radians.</param>
		/// <param name="tanHalfFov">The fov.</param>
		/// <returns>True if the angle is within the FOV, false otherwise.</returns>
		static bool AngleInFov(float angle, float tanHalfFov);

		/// <summary>
		/// Projects an angle onto the screen for rendering.
		/// </summary>
		/// <param name="angle">Angle to project in radians.</param>
		/// <param name="tanHalfFov">The fov.</param>
		/// <returns>Screen projection of the angle.</returns>
		static float AngleScreenProjection(float angle, float tanHalfFov);

		/// <summary>
		/// Converts a range specified by start and end angles to a range structure based on yaw.
		/// </summary>
		/// <param name="start">Starting angle in radians.</param>
		/// <param name="end">Ending angle in radians.</param>
		/// <param name="yaw">Yaw angle in radians.</param>
		/// <param name="tanHalfFov">The fov.</param>
		static vec3 AnglesToRange(float start, float end, float yaw, float tanHalfFov);

		/// <summary>
		/// Converts Euler angles to a forward, right, and up vector.
		/// </summary>
		/// <param name="angles">Euler angles in radians.</param>
		/// <param name="forward">Output forward vector.</param>
		/// <param name="right">Output right vector.</param>
		/// <param name="up">Output up vector.</param>
		static void AngleVectors(const float* angles, float* forward, float* right, float* up);

		/// <summary>
		/// Converts Euler angles to a up vector.
		/// </summary>
		/// <param name="angles">Euler angles in radians.</param>
		/// <returns>Up vector.</returns>
		static vec3 AnglesToUp(const float* angles);

		/// <summary>
		/// Converts Euler angles to a forward vector.
		/// </summary>
		/// <param name="angles">Euler angles in radians.</param>
		/// <returns>Forward vector.</returns>
		static vec3 AnglesToForward(const float* angles);

		/// <summary>
		/// Converts Euler angles to a right vector.
		/// </summary>
		/// <param name="angles">Euler angles in radians.</param>
		/// <returns>Right vector.</returns>
		static vec3 AnglesToRight(const float* angles);

		/// <summary>
		/// Angular distance between 2 values.
		/// </summary>
		/// <param name="value1">1st value.</param>
		/// <param name="value2">2nd value.</param>
		/// <returns></returns>
		static float AngularDistance(float value1, float value2);

		/// <summary>
		/// Converts a 3D vector to Euler angles (pitch, yaw, and roll) in radians.
		/// </summary>
		/// <param name="v">Input vector.</param>
		/// <returns>Euler angles in radians.</returns>
		static vec3 VectorToAngles(const float* v);

		/// <summary>
		/// Converts a forward and up vector to Euler angles (pitch, yaw, and roll) in radians.
		/// </summary>
		/// <param name="forward">Forward vector.</param>
		/// <param name="up">Up vector.</param>
		/// <param name="flipPitch">True to flip the pitch angle.</param>
		static vec3 VectorToAnglesWithRoll(const float* forward, const float* up, bool flipPitch);

		/// <summary>
		/// Normalize vector.
		/// </summary>
		/// <param name="v">The vector.</param>
		/// <returns>Length of the original vector.</returns>
		static float VectorNormalize3(float* v);

		/// <summary>
		/// Lerp vector.
		/// </summary>
		/// <param name="start">Start value.</param>
		/// <param name="end">End value.</param>
		/// <param name="fraction">The fraction.</param>
		/// <param name="out">The output variable.</param>
		static void VectorLerp3(const float* start, const float* end, float fraction, float* out);
	};
}

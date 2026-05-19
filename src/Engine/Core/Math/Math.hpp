#pragma once
#include "Engine/Core/Interfaces/ISerializable.hpp"

#define GLM_FORCE_CTOR_INIT
#define GLM_FORCE_XYZW_ONLY
#define GLM_FORCE_LEFT_HANDED
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat3 = glm::mat3;
using mat4 = glm::mat4;

#define M_PI 3.14159265358979323846f
#define ANGLE2SHORT(x) (static_cast<int>((x) * 65536 / 360) & 65535)
#define SHORT2ANGLE(x) (x * (360.0f / 65536))

#define PITCH 0
#define YAW 1
#define ROLL 2

namespace glm
{
	SERIALIZE_NON_INTRUSIVE(vec2, x, y)
	SERIALIZE_NON_INTRUSIVE(vec3, x, y, z)
	SERIALIZE_NON_INTRUSIVE(vec4, x, y, z, w)
}

namespace IzEngine
{
	class API Math
	{
	public:
		static bool Contains(const vec2& point, const vec2& size);

		static float RadToDeg(float radians);
		static float DegToRad(float degrees);

		static float AngleNormalize90(float angle);
		static float AngleNormalize180(float angle);
		static float AngleNormalize360(float angle);
		static float AngleNormalizePI(float angle);

		static float AngleDelta(float angle1, float angle2);
		static bool AngleInFov(float angle, float tanHalfFov);
		static float AngleScreenProjection(float angle, float tanHalfFov);
		static vec3 AnglesToRange(float start, float end, float yaw, float tanHalfFov);
		static void AngleVectors(const vec3& angles, vec3& forward, vec3& right, vec3& up);
		static vec3 AnglesToUp(const vec3& angles);
		static vec3 AnglesToForward(const vec3& angles);
		static vec3 AnglesToRight(const vec3& angles);
		static float AngularDistance(float value1, float value2);

		static vec3 VectorToAngles(const vec3& v);

		static int BGRA(const vec4& v);
		static int RGBA(const vec4& v);
	};
}

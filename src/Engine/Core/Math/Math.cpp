#include "Math.hpp"

#include "Engine/Renderer/UI/VirtualScreen.hpp"

namespace IzEngine
{
	bool Math::Contains(const vec2& point, const vec2& size)
	{
		return point.x >= 0.0f && point.x < size.x && point.y >= 0.0f && point.y < size.y;
	}

	float Math::RadToDeg(float radians)
	{
		return radians * (180.f / M_PI);
	}

	float Math::DegToRad(float degrees)
	{
		return degrees * (M_PI / 180.f);
	}

	float Math::AngleNormalize360(float angle)
	{
		angle = std::fmod(angle, 360.f);
		return angle < 0.f ? angle + 360.f : angle;
	}

	float Math::AngleNormalize180(float angle)
	{
		angle = AngleNormalize360(angle);
		return angle > 180.f ? angle - 360.f : angle;
	}

	float Math::AngleNormalize90(float angle)
	{
		angle = std::fmod(angle, 180.f);
		if (angle > 90.f)
			angle -= 180.f;
		if (angle < -90.f)
			angle += 180.f;
		return angle;
	}

	float Math::AngleNormalizePI(float angle)
	{
		angle = std::fmod(angle + M_PI, 2.f * M_PI);
		if (angle < 0.f)
			angle += 2.f * M_PI;
		return angle - M_PI;
	}

	float Math::AngleDelta(float angle1, float angle2)
	{
		return AngleNormalize180(angle1 - angle2);
	}

	bool Math::AngleInFov(float angle, float tanHalfFov)
	{
		const float half_fov_x = std::atan(tanHalfFov);
		return angle > -half_fov_x && angle < half_fov_x;
	}

	float Math::AngleScreenProjection(float angle, float tanHalfFov)
	{
		const float half_fov_x = std::atan(tanHalfFov);

		if (angle >= half_fov_x)
			return 0;
		if (angle <= -half_fov_x)
			return SCREEN_WIDTH;

		return SCREEN_WIDTH * 0.5f * (1.f - std::tan(angle) / std::tan(half_fov_x));
	}

	vec3 Math::AnglesToRange(float start, float end, float yaw, float tanHalfFov)
	{
		if (std::abs(end - start) > 2.f * M_PI)
			return { 0, SCREEN_WIDTH, 0 };

		bool split = end > start;

		start = AngleNormalizePI(start - yaw);
		end = AngleNormalizePI(end - yaw);

		if (end > start)
		{
			split = !split;
			std::swap(start, end);
		}

		return { AngleScreenProjection(start, tanHalfFov), AngleScreenProjection(end, tanHalfFov),
			static_cast<float>(split) };
	}

	void Math::AngleVectors(const vec3& angles, vec3& forward, vec3& right, vec3& up)
	{
		float angle;
		float sr, sp, sy, cr, cp, cy;

		angle = angles[YAW] * (M_PI * 2.f / 360.f);
		sy = std::sin(angle);
		cy = std::cos(angle);

		angle = angles[PITCH] * (M_PI * 2.f / 360.f);
		sp = std::sin(angle);
		cp = std::cos(angle);

		angle = angles[ROLL] * (M_PI * 2.f / 360.f);
		sr = std::sin(angle);
		cr = std::cos(angle);

		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;

		right[0] = -sr * sp * cy + -cr * -sy;
		right[1] = -sr * sp * sy + -cr * cy;
		right[2] = -sr * cp;

		up[0] = cr * sp * cy + -sr * -sy;
		up[1] = cr * sp * sy + -sr * cy;
		up[2] = cr * cp;
	}

	vec3 Math::AnglesToUp(const vec3& angles)
	{
		float angle;
		float sr, sp, sy, cr, cp, cy;

		angle = angles[YAW] * (M_PI * 2.f / 360.f);
		sy = std::sin(angle);
		cy = std::cos(angle);

		angle = angles[PITCH] * (M_PI * 2.f / 360.f);
		sp = std::sin(angle);
		cp = std::cos(angle);

		angle = angles[ROLL] * (M_PI * 2.f / 360.f);
		sr = std::sin(angle);
		cr = std::cos(angle);

		return { cr * sp * cy + -sr * -sy, cr * sp * sy + -sr * cy, cr * cp };
	}

	vec3 Math::AnglesToForward(const vec3& angles)
	{
		float angle;
		float sp, sy, cp, cy;

		angle = angles[YAW] * (M_PI * 2.f / 360.f);
		sy = std::sin(angle);
		cy = std::cos(angle);

		angle = angles[PITCH] * (M_PI * 2.f / 360.f);
		sp = std::sin(angle);
		cp = std::cos(angle);

		return { cp * cy, cp * sy, -sp };
	}

	vec3 Math::AnglesToRight(const vec3& angles)
	{
		float angle;
		float sr, sp, sy, cr, cp, cy;

		angle = angles[YAW] * (M_PI * 2.f / 360.f);
		sy = std::sin(angle);
		cy = std::cos(angle);

		angle = angles[PITCH] * (M_PI * 2.f / 360.f);
		sp = std::sin(angle);
		cp = std::cos(angle);

		angle = angles[ROLL] * (M_PI * 2.f / 360.f);
		sr = std::sin(angle);
		cr = std::cos(angle);

		return { -sr * sp * cy + -cr * -sy, -sr * sp * sy + -cr * cy, -sr * cp };
	}

	float Math::AngularDistance(float value1, float value2)
	{
		float diff = std::fmod(value2 - value1 + 180.f, 360.f) - 180.f;
		if (diff < -180.f)
			diff += 360.f;
		return std::abs(diff);
	}

	vec3 Math::VectorToAngles(const vec3& v)
	{
		vec3 angles;
		float yaw, pitch;

		if (v[0] == 0.f && v[1] == 0.f)
		{
			yaw = 0.f;
			pitch = v[2] > 0.f ? 90.f : 270.f;
		}
		else
		{
			yaw = std::atan2(v[1], v[0]) * 180.f / M_PI;
			if (yaw < 0.f)
				yaw += 360.f;

			pitch = std::atan2(v[2], glm::length(vec2(v))) * 180.f / M_PI;
			if (pitch < 0.f)
				pitch += 360.f;
		}
		angles[PITCH] = -pitch;
		angles[YAW] = yaw;
		angles[ROLL] = 0.f;
		return angles;
	}

	int Math::BGRA(const vec4& v)
	{
		int color = 0;
		color |= (int(v.w * 255.f) & 0xFF) << 24;
		color |= (int(v.x * 255.f) & 0xFF) << 16;
		color |= (int(v.y * 255.f) & 0xFF) << 8;
		color |= (int(v.z * 255.f) & 0xFF);
		return color;
	}

	int Math::RGBA(const vec4& v)
	{
		int color = 0;
		color |= (int(v.w * 255.f) & 0xFF) << 24;
		color |= (int(v.z * 255.f) & 0xFF) << 16;
		color |= (int(v.y * 255.f) & 0xFF) << 8;
		color |= (int(v.x * 255.f) & 0xFF);
		return color;
	}
}

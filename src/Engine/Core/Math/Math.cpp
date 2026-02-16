#include "Math.hpp"

#include "Matrix3.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace IzEngine
{
	float Math::RadToDeg(float radians)
	{
		return radians * (180.0f / M_PI);
	}

	float Math::DegToRad(float degrees)
	{
		return degrees * M_PI / 180.0f;
	}

	float Math::AngleNormalize90(float angle)
	{
		return fmod(angle + 180 + 90, 180) - 90;
	}

	float Math::AngleNormalize180(float angle)
	{
		angle = AngleNormalize360(angle);
		if (angle > 180.0)
			angle -= 360.0;
		return angle;
	}

	float Math::AngleNormalize360(float angle)
	{
		return (360.0f / 65536) * (static_cast<int>(angle * (65536 / 360.0f)) & 65535);
	}

	float Math::AngleNormalizePI(float angle)
	{
		const float tAngle = fmod(angle + M_PI, 2 * M_PI);
		return tAngle < 0 ? tAngle + M_PI : tAngle - M_PI;
	}

	float Math::AngleDelta(float angle1, float angle2)
	{
		return AngleNormalize360(angle1 - angle2);
	}

	bool Math::AngleInFov(float angle, float tanHalfFov)
	{
		const float half_fov_x = atan(tanHalfFov);
		return angle > -half_fov_x && angle < half_fov_x;
	}

	float Math::AngleScreenProjection(float angle, float tanHalfFov)
	{
		const float half_fov_x = atan(tanHalfFov);

		if (angle >= half_fov_x)
			return 0;
		if (angle <= -half_fov_x)
			return SCREEN_WIDTH;

		return SCREEN_WIDTH / 2.f * (1.f - tan(angle) / tan(half_fov_x));
	}

	vec3 Math::AnglesToRange(float start, float end, float yaw, float tanHalfFov)
	{
		if (abs(end - start) > 2.f * M_PI)
			return { 0, SCREEN_WIDTH, 0 };
		bool split = end > start;

		start = AngleNormalizePI(start - yaw);
		end = AngleNormalizePI(end - yaw);

		if (end > start)
		{
			split = !split;
			const float tmp = start;

			start = end;
			end = tmp;
		}
		return { AngleScreenProjection(start, tanHalfFov), AngleScreenProjection(end, tanHalfFov),
			static_cast<float>(split) };
	}

	void Math::AngleVectors(const float* angles, float* forward, float* right, float* up)
	{
		float angle;
		float sr, sp, sy, cr, cp, cy;

		angle = angles[YAW] * (M_PI * 2.0f / 360.0f);
		sy = sin(angle);
		cy = cos(angle);

		angle = angles[PITCH] * (M_PI * 2.0f / 360.0f);
		sp = sin(angle);
		cp = cos(angle);

		angle = angles[ROLL] * (M_PI * 2.0f / 360.0f);
		sr = sin(angle);
		cr = cos(angle);

		if (forward)
		{
			forward[0] = cp * cy;
			forward[1] = cp * sy;
			forward[2] = -sp;
		}
		if (right)
		{
			right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
			right[1] = -1 * sr * sp * sy + -1 * cr * cy;
			right[2] = -1 * sr * cp;
		}
		if (up)
		{
			up[0] = cr * sp * cy + -sr * -sy;
			up[1] = cr * sp * sy + -sr * cy;
			up[2] = cr * cp;
		}
	}

	vec3 Math::AnglesToUp(const float* angles)
	{
		float angle;
		float sr, sp, sy, cr, cp, cy;

		angle = angles[YAW] * (M_PI * 2.0f / 360.0f);
		sy = sin(angle);
		cy = cos(angle);

		angle = angles[PITCH] * (M_PI * 2.0f / 360.0f);
		sp = sin(angle);
		cp = cos(angle);

		angle = angles[ROLL] * (M_PI * 2.0f / 360.0f);
		sr = sin(angle);
		cr = cos(angle);

		return { cr * sp * cy + -sr * -sy, cr * sp * sy + -sr * cy, cr * cp };
	}

	vec3 Math::AnglesToForward(const float* angles)
	{
		float angle;
		float sp, sy, cp, cy;

		angle = angles[YAW] * (M_PI * 2.0f / 360.0f);
		sy = sin(angle);
		cy = cos(angle);

		angle = angles[PITCH] * (M_PI * 2.0f / 360.0f);
		sp = sin(angle);
		cp = cos(angle);

		return { cp * cy, cp * sy, -sp };
	}

	vec3 Math::AnglesToRight(const float* angles)
	{
		float angle;
		float sr, sp, sy, cr, cp, cy;

		angle = angles[YAW] * (M_PI * 2.0f / 360.0f);
		sy = sin(angle);
		cy = cos(angle);

		angle = angles[PITCH] * (M_PI * 2.0f / 360.0f);
		sp = sin(angle);
		cp = cos(angle);

		angle = angles[ROLL] * (M_PI * 2.0f / 360.0f);
		sr = sin(angle);
		cr = cos(angle);

		return { -1 * sr * sp * cy + -1 * cr * -sy, -1 * sr * sp * sy + -1 * cr * cy, -1 * sr * cp };
	}

	void Math::AnglesToAxis(const vec3& angles, float axis[3][3])
	{
		float pitch = angles.x;
		float yaw = angles.y;
		float roll = angles.z;

		float sp = sinf(pitch * (3.14159265f / 180.0f));
		float cp = cosf(pitch * (3.14159265f / 180.0f));
		float sy = sinf(yaw * (3.14159265f / 180.0f));
		float cy = cosf(yaw * (3.14159265f / 180.0f));
		float sr = sinf(roll * (3.14159265f / 180.0f));
		float cr = cosf(roll * (3.14159265f / 180.0f));

		axis[0][0] = cp * cy;
		axis[0][1] = cp * sy;
		axis[0][2] = -sp;

		axis[1][0] = sr * sp * cy - cr * sy;
		axis[1][1] = sr * sp * sy + cr * cy;
		axis[1][2] = sr * cp;

		axis[2][0] = cr * sp * cy + sr * sy;
		axis[2][1] = cr * sp * sy - sr * cy;
		axis[2][2] = cr * cp;
	}

	float Math::AngularDistance(float value1, float value2)
	{
		float diff = fmod(value2 - value1 + 180, 360) - 180;
		if (diff < -180)
			diff += 360;
		return std::abs(diff);
	}

	float Math::SignedAngularDistance(float value1, float value2)
	{
		float diff = fmod(value2 - value1 + 180, 360) - 180;
		if (diff < -180)
			diff += 360;
		return diff;
	}

	vec3 Math::VectorToAngles(const float* v)
	{
		vec3 angles;
		float yaw, pitch;

		if (v[1] == 0.0f && v[0] == 0.0f)
		{
			yaw = 0.0f;
			if (v[2] > 0)
				pitch = 90.0f;
			else
				pitch = 270.0f;
		}
		else
		{
			if (v[0])
				yaw = atan2(v[1], v[0]) * 180.0f / M_PI;
			else if (v[1] > 0)
				yaw = 90.0f;
			else
				yaw = 270.0f;

			if (yaw < 0)
				yaw += 360.0f;

			const float forward = VectorLength3(v);
			pitch = atan2(v[2], forward) * 180.0f / M_PI;

			if (pitch < 0)
				pitch += 360.0f;
		}
		angles[PITCH] = -pitch;
		angles[YAW] = yaw;
		angles[ROLL] = 0.0f;
		return angles;
	}

	vec3 Math::VectorToAnglesWithRoll(const float* forward, const float* up, bool flipPitch)
	{
		vec3 angles;
		if (forward[0] == 0.0f && forward[1] == 0.0f)
		{
			if (forward[2] > 0.0f)
			{
				angles[PITCH] = -M_PI * 0.5f;
				angles[YAW] = up[0] ? atan2(-up[1], -up[0]) : 0.0f;
			}
			else
			{
				angles[PITCH] = M_PI * 0.5f;
				angles[YAW] = up[0] ? atan2(up[1], up[0]) : 0.0f;
			}
			angles[ROLL] = 0.0f;
		}
		else
		{
			angles[YAW] = atan2(forward[1], forward[0]);
			angles[PITCH] = -atan2(forward[2], sqrt(forward[0] * forward[0] + forward[1] * forward[1]));

			if (up[0])
			{
				const float cp = cos(angles[PITCH]);
				const float sp = sin(angles[PITCH]);
				const float cy = cos(angles[YAW]);
				const float sy = sin(angles[YAW]);

				const vec3 tleft = { -sy, cy, 0.0f };
				const vec3 tup = { (sp * cy), (sp * sy), (cp) };

				angles[ROLL] = -atan2(DotProduct3(up, tleft), DotProduct3(up, tup));
			}
			else
				angles[ROLL] = 0;
		}
		angles *= 180.0f / M_PI;

		if (flipPitch)
			angles[PITCH] *= -1.0f;
		if (angles[PITCH] < 0)
			angles[PITCH] += 360.0f;
		if (angles[YAW] < 0)
			angles[YAW] += 360.0f;
		if (angles[ROLL] < 0)
			angles[ROLL] += 360.0f;

		return angles;
	}

	float Math::VectorNormalize3(float* v)
	{
		const float len = VectorLength3(v);
		if (len != 0.0f)
			VectorScale3(v, 1.0f / len, v);
		return len;
	}

	void Math::VectorLerp3(const float* start, const float* end, float fraction, float* out)
	{
		if (fraction == 1.0f)
			VectorCopy3(end, out);
		else
		{
			out[0] = start[0] + fraction * (end[0] - start[0]);
			out[1] = start[1] + fraction * (end[1] - start[1]);
			out[2] = start[2] + fraction * (end[2] - start[2]);
		}
	}
}

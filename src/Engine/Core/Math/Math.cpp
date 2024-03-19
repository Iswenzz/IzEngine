#include "Math.hpp"

#include "Matrix3.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace IW3SR::Engine
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

    float Math::AngleNormalizePI(const float angle)
    {
        const float tAngle = fmod(angle + M_PI, 2 * M_PI);
        return tAngle < 0 ? tAngle + M_PI : tAngle - M_PI;
    }

    float Math::AngleDelta(float angle1, float angle2)
    {
        return AngleNormalize360(angle1 - angle2);
    }

    void Math::AngleVectors(const vec3& angles, vec3& forward, vec3& right, vec3& up)
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

    vec3 Math::AnglesToUp(const vec3& angles)
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

        return {
            cr * sp * cy + -sr * -sy,
            cr * sp * sy + -sr * cy,
            cr * cp
        };
    }

    vec3 Math::AnglesToForward(const vec3& angles)
    {
        float angle;
        float sp, sy, cr, cp, cy;

        angle = angles[YAW] * (M_PI * 2.0f / 360.0f);
        sy = sin(angle);
        cy = cos(angle);

        angle = angles[PITCH] * (M_PI * 2.0f / 360.0f);
        sp = sin(angle);
        cp = cos(angle);

        return {
            cp * cy,
            cp * sy,
            -sp
        };
    }

    vec3 Math::AnglesToRight(const vec3& angles)
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

        return {
            -1 * sr * sp * cy + -1 * cr * -sy,
            -1 * sr * sp * sy + -1 * cr * cy,
            -1 * sr * cp
        };
    }

    vec3 Math::VectorToAngles(const vec3& v)
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

            const float forward = v.Length();
            pitch = atan2(v[2], forward) * 180.0f / M_PI;

            if (pitch < 0)
                pitch += 360.0f;
        }
        angles[PITCH] = -pitch;
        angles[YAW] = yaw;
        angles[ROLL] = 0.0f;
        return angles;
    }

    vec3 Math::VectorToAnglesWithRoll(const vec3& forward, const vec3& up, bool flipPitch)
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
                const float cp = cos(angles[PITCH]), sp = sin(angles[PITCH]);
                const float cy = cos(angles[YAW]), sy = sin(angles[YAW]);

                const vec3 tleft = {
                    -sy, cy, 0.0f
                };
                const vec3 tup = {
                    (sp * cy),
                    (sp * sy),
                    (cp)
                };

                angles[ROLL] = -atan2(up.Dot(tleft), up.Dot(tup));
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

		return SCREEN_WIDTH / 2 * (1 - tan(angle) / tan(half_fov_x));
	}

	vec3 Math::AnglesToRange(float start, float end, float yaw, float tanHalfFov)
	{
		if (abs(end - start) > 2 * M_PI)
			return { 0, SCREEN_WIDTH, false };
		float split = end > start;

		start = AngleNormalizePI(start - yaw);
		end = AngleNormalizePI(end - yaw);

		if (end > start)
		{
			split = !split;
			const float tmp = start;

			start = end;
			end = tmp;
		}
		return { AngleScreenProjection(start, tanHalfFov), AngleScreenProjection(end, tanHalfFov), split };
	}
}

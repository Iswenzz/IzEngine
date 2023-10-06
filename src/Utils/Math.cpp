#include "Math.hpp"

namespace IW3SR
{
    vec2 WorldToScreen(const vec3& worldPosition)
    {
        const int centerX = cgs->refdef.width / 2;
        const int centerY = cgs->refdef.height / 2;

        const mat3 viewAxis = cgs->refdef.viewaxis;
        const vec3 origin = cgs->refdef.vieworg;
        const vec3 local = worldPosition - origin;

        const vec3 forward = viewAxis[0];
        const vec3 right = viewAxis[1];
        const vec3 up = viewAxis[2];

        const vec3 transform = {
            local.Dot(right),
            local.Dot(up),
            local.Dot(forward)
        };

        if (transform[2] < 0.01)
            return vec2{ };

        return {
            centerX * (1 - (transform[0] / cgs->refdef.tanHalfFovX / transform[2])),
            centerY * (1 - (transform[1] / cgs->refdef.tanHalfFovY / transform[2]))
        };
    }

    float RadToDeg(const float radians)
    {
        return radians * (180.0f / M_PI);
    }

    float DegToRad(const float degrees)
    {
        return degrees * M_PI / 180.0f;
    }

    float AngleNormalize180(const float angle)
    {
        return (angle * 0.0027777778f - floorf(angle * 0.0027777778f + 0.5f)) * 360.0f;
    }

    float AngleNormalizePi(const float angle)
    {
        const float tAngle = fmodf(angle + M_PI, 2 * M_PI);
        return tAngle < 0 ? tAngle + M_PI : tAngle - M_PI;
    }

    void AngleVectors(const vec3& angles, vec3& forward, vec3& right, vec3& up)
    {
        float angle;
        static float sr, sp, sy, cr, cp, cy;

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

    vec3 AnglesToUp(const vec3& angles)
    {
        float angle;
        static float sr, sp, sy, cr, cp, cy;

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

    vec3 AngleToForward(const vec3& angles)
    {
        float angle;
        static float sp, sy, cr, cp, cy;

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

    vec3 AngleToRight(const vec3& angles)
    {
        float angle;
        static float sr, sp, sy, cr, cp, cy;

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

    vec3 VectorToAngles(const vec3& v)
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

    vec3 VectorToAnglesWithRoll(const vec3& forward, const vec3& up, bool flipPitch)
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
 
	void AdjustFrom640(float& x, float& y, float& w, float& h)
	{
		const float scaleX = static_cast<float>(cgs->refdef.width) / 640.0f;

		x *= scaleX;
		y *= scaleX;
		w *= scaleX;
		h *= scaleX;
	}

	void FillRect(float x, float y, float w, float h, const vec4& color)
	{
		if (!w || !h)
			return;

		AdjustFrom640(x, y, w, h);

		const auto material = Material_RegisterHandle("white", 3);
		R_AddCmdDrawStretchPic(material, x, y, w, h, 0.0f, 0.0f, 0.0f, 0.0f, color);
	}

	void FillAngleYaw(float start, float end, float yaw, float y, float h, const vec4& color)
	{
		const range_t range = AnglesToRange(start, end, yaw);
		if (!range.split)
			FillRect(range.x1, y, range.x2 - range.x1, h, color);
		else
		{
			FillRect(0, y, range.x1, h, color);
			FillRect(range.x2, y, SCREEN_WIDTH - range.x2, h, color);
		}
	}

	void DrawLineYaw(float angle, float yaw, float y, float w, float h, const vec4& color)
	{
		angle = AngleNormalizePi(angle - yaw);
		if (!AngleInFov(angle))
			return;

		float const x = AngleScreenProjection(angle);
		FillRect(x - w / 2, y, w, h, color);
	}

	bool AngleInFov(float angle)
	{
		float const half_fov_x = atanf(cgs->refdef.tanHalfFovX);
		return angle > -half_fov_x && angle < half_fov_x;
	}

	float AngleScreenProjection(float angle)
	{
		float const half_fov_x = atanf(cgs->refdef.tanHalfFovX);

		if (angle >= half_fov_x)
			return 0;
		if (angle <= -half_fov_x)
			return SCREEN_WIDTH;

		return SCREEN_WIDTH / 2 * (1 - tanf(angle) / tanf(half_fov_x));
	}

	range_t AnglesToRange(float start, float end, float yaw)
	{
		if (fabsf(end - start) > 2 * M_PI)
			return { 0, SCREEN_WIDTH, false };
		bool split = end > start;

		start = AngleNormalizePi(start - yaw);
		end = AngleNormalizePi(end - yaw);

		if (end > start)
		{
			split = !split;
			float const tmp = start;

			start = end;
			end = tmp;
		}
		return { AngleScreenProjection(start), AngleScreenProjection(end), split };
	}
}

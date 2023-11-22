#include "PMove.hpp"
#include "Modules/Modules.hpp"

namespace IW3SR
{
	void PMove::FinishMove(usercmd_s* cmd)
	{
		ModuleCallback(OnFinishMove, cmd);
		CL_FinishMove_h(cmd);
	}

	void PMove::SetYaw(usercmd_s* cmd, float* angles, const vec3& target)
    {
        float cmdAngle = SHORT2ANGLE(cmd->angles[1]);

        float delta = Math::AngleDelta(angles[1], cmdAngle);
        float realDelta = Math::AngleDelta(delta, angles[1]);
        float final = Math::AngleDelta(angles[1], target.y);

        clients->viewangles[1] += realDelta - final;
        cmd->angles[1] += ANGLE2SHORT(realDelta - final);
    }

    void PMove::SetPitch(usercmd_s* cmd, float* angles, const vec3& target)
    {
        float cmdAngle = SHORT2ANGLE(cmd->angles[0]);

        float delta = Math::AngleDelta(angles[0], cmdAngle);
        float realDelta = Math::AngleDelta(delta, angles[0]);
        float final = Math::AngleDelta(angles[0], target.x);

        clients->viewangles[0] += realDelta - final;
        cmd->angles[0] += ANGLE2SHORT(realDelta - final);
    }
}

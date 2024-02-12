#include "PMove.hpp"

namespace IW3SR::Game
{
	void PMove::FinishMove(usercmd_s* cmd)
	{
        CL_FinishMove_h(cmd);
		ModulesCallback(OnFinishMove, cmd);
	}

    void PMove::WalkMove(pmove_t* pm, pml_t* pml)
    {
        ModulesCallback(OnWalkMove, pm, pml);
        PM_WalkMove_h(pm, pml);
    }

    void PMove::AirMove(pmove_t* pm, pml_t* pml)
    {
        ModulesCallback(OnAirMove, pm, pml);
        PM_AirMove_h(pm, pml);
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

    void PMove::SetAngles(usercmd_s* cmd, float* angles, const vec3& target)
    {
        SetPitch(cmd, angles, target);
        SetYaw(cmd, angles, target);
    }

    bool PMove::OnGround()
    {
        return pmove->ps->groundEntityNum != 1023;
    }

    bool PMove::InAir()
    {
        return pmove->ps->groundEntityNum == 1023;
    }
}

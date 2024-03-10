#include "Player.hpp"

namespace IW3SR::Game
{
	Player::Player(int index)
	{
		c = &cg_entities[index];
		info = &cgs->bgs.clientinfo[index];
	}

	void Player::Initialize()
	{
		for (int i = 0; i < Players.size(); i++)
			Players[i] = CreateRef<Player>(i);
	}

	bool Player::IsSelf()
	{
		return c->nextState.clientNum == cgs->clientNum;
	}

	bool Player::IsAlive()
	{
		return c->isAlive;
	}

	bool Player::OnGround()
	{
		return c->nextState.groundEntityNum != 1023;
	}

	bool Player::InAir()
	{
		return c->nextState.groundEntityNum == 1023;
	}

	void Player::Predict(int localClientNum)
	{
		CG_PredictPlayerState_Internal_h(localClientNum);
		InterpolateViewForMover();
	}

	void Player::InterpolateViewForMover()
	{
		const centity_s* cent = &cg_entities[cgs->predictedPlayerState.groundEntityNum];
		const entityType_t eType = cent->nextState.eType;

		auto viewAngles = cgs->predictedPlayerState.viewangles;
		auto deltaAngles = cgs->predictedPlayerState.delta_angles;
		const int fromTime = cgs->snap->serverTime;
		const int toTime = cgs->time;

		if (eType == ET_SCRIPTMOVER || eType == ET_PLANE)
		{
			vec3 angles, oldAngles;
			BG_EvaluateTrajectory(&cent->currentState.apos, fromTime, oldAngles);
			BG_EvaluateTrajectory(&cent->currentState.apos, toTime, angles);
			vec3 delta = angles - oldAngles;

			viewAngles[0] += delta.x;
			viewAngles[1] += delta.y;
			viewAngles[2] += delta.z;

			deltaAngles[0] += delta.x;
			deltaAngles[1] += delta.y;
			deltaAngles[2] += delta.z;
		}
	}

	std::array<Ref<Player>, 64>& Player::GetAll()
	{
		return Players;
	}

	Ref<Player>& Player::Get(int index)
	{
		return Players[index];
	}

	Ref<Player>& Player::Self()
	{
		return Players[cgs->clientNum];
	}

	Player::operator bool() const
	{
		return c;
	}
}

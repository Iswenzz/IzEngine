#include "Player.hpp"
#include "Game/Game.hpp"

namespace IW3SR::Game
{
	Player::Player(int index)
	{
		c = &cg_entities[index];
		info = &cgs->bgs.clientinfo[index];
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

	void Player::Allocates()
	{
		for (int i = 0; i < Players.size(); i++)
			Players[i] = std::make_shared<Player>(i);
	}

	void Player::Interpolate(bool grabAngles)
	{
		CG_InterpolatePlayerState_h(grabAngles);
	}

	std::array<std::shared_ptr<Player>, 64>& Player::GetAll()
	{
		return Players;
	}

	std::shared_ptr<Player>& Player::Get(int index)
	{
		return Players[index];
	}

	std::shared_ptr<Player>& Player::Self()
	{
		return Players[cgs->clientNum];
	}

	Player::operator bool() const
	{
		return c;
	}
}

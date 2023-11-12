#include "Player.hpp"
#include "Game/Game.hpp"

namespace IW3SR
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

	void Player::Allocates()
	{
		for (int i = 0; i < 64; i++) 
			SR->Players[i] = std::make_shared<Player>(i);
	}

	std::array<std::shared_ptr<Player>, 64>& Player::GetAll()
	{
		return SR->Players;
	}

	std::shared_ptr<Player> Player::Get(int index)
	{
		return SR->Players[index];
	}

	bool Player::CanRender()
	{
		return cgs->predictedPlayerState.groundEntityNum;
	}

	Player::operator bool() const
	{
		return c;
	}
}

#include "Player.h"

Player& Player::GetInstance()
{
	static Player player;
	return player;
}

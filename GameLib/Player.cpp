#include "Player.h"
#include <random>


Player::Player()
{
	life = 0;
	rol = 0;
	
}

Player::~Player()
{
}

void Player::GetAction(int num, Boss boss) {
	switch (num)
	{
	case 1:
		Action1(boss);
		break;
	case 2:
		Action2(boss);
		break;
	case 3:	
		Action3(boss);
		break;
	default:
		break;
	}
}


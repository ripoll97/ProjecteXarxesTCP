#include "Player.h"
#include <random>


Player::Player()
{
	maxLife = 0;
	life = 0;

}

Player::~Player()
{
}

void Player::Action1(Boss boss) {}

void Player::Action2(Boss boss) {}

void Player::Action3(Boss boss) {}


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


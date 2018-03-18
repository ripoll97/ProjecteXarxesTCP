#pragma once
#include <SFML\Graphics.hpp>
//#include "Player.h"


class Boss
{

public:
	int maxLife;
	int life;
	bool tankAggro;
	int singleDmg, doubleDmg, multipleDmg;
	Boss();
	//void Boss::Action1(std::vector<Player>aPlayers);
	//void Boss::Action2(std::vector<Player>aPlayers);
	//void Boss::Action3(std::vector<Player>aPlayers);
	~Boss();
};
#pragma once
#include <SFML\Graphics.hpp>
#include "Boss.h"

class Player
{

public:
	int maxLife;
	int life;

	Player();
	void GetAction(int num, Boss boss);
	virtual void Action1(Boss boss);
	virtual void Action2(Boss boss);
	virtual void Action3(Boss boss);
	~Player();
};
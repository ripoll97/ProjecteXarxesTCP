#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Healer : Player
{


public:
	Healer();
	getAction(int num, Boss myBoss)
	{
		switch
			case 1:
				Action1(myBoss);
	}

	Action1(Boss myBoss) {
		Boss -=
	}
	~Healer();
};
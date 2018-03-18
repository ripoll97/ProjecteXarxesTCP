#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Tank : public Player
{


public:
	
	Tank();
	void Tank::Action1(Boss boss);

	void Tank::Action2(Boss boss);

	void Tank::Action3(Boss boss);
	~Tank();
};
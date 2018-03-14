#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Archer : Player
{
	

public:
	Archer();
	void Archer::Action1(Boss boss);

	void Archer::Action2(Boss boss);

	void Archer::Action3(Boss boss);
	~Archer();
};
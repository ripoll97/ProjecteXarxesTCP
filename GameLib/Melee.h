#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Melee : public Player
{


public:
	Melee();
	void Melee::Action1(Boss boss);

	void Melee::Action2(Boss boss);

	void Melee::Action3(Boss boss);
	~Melee();
};
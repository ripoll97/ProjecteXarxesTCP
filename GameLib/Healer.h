#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Healer : public Player
{


public:
	Healer();
	void Healer::Action1(Boss boss);

	void Healer::Action2(Boss boss);

	void Healer::Action3(Boss boss);

	~Healer();
};
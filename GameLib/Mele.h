#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Mele : Player
{


public:
	Mele();
	void Mele::Action1(Boss boss);

	void Mele::Action2(Boss boss);

	void Mele::Action3(Boss boss);
	~Mele();
};
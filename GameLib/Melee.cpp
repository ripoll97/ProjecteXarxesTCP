#include "Melee.h"

Melee::Melee()
{
	combo = false;
	isBuffed = false;
	maxLife = 100;
	life = maxLife;
}

Melee::~Melee()
{
}

void Melee::Action1(Boss boss) {
	if (isBuffed) {
		boss.life -= 5;
	}
	else {
		boss.life -= 3;
	}
	combo = true;
}

void Melee::Action2(Boss boss) {
	if (isBuffed) {
		if (combo) {
			boss.life -= 6;
			life += 4;
			if (life > maxLife) { life = maxLife; }
			combo = false;
		}
		else {
			boss.life -= 2;
			life += 4;
			if (life > maxLife) { life = maxLife; }
		}
		isBuffed = false;
	}
	else {
		if (combo) {
			boss.life -= 6;
			combo = false;
		}
		else {
			boss.life -= 2;
		}
	}
}

void Melee::Action3(Boss boss) {
	isBuffed = true;
}
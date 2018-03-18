#include "Archer.h"

Archer::Archer()
{
	combo = false;
	isBuffed = false;
	maxLife = 75;
	life = maxLife;
	bonusDmg = 2;
}

Archer::~Archer()
{
}
//Normal attack
void Archer::Action1(Boss boss) {
	if (isBuffed) {
		boss.life -= 3 * bonusDmg;
		life += 2;
	}
	else {
		boss.life -= 3;
	}
	combo = true;
}
//Combo attack
void Archer::Action2(Boss boss) {
	if (isBuffed) {
		if (combo) {
			boss.life -= 4 * bonusDmg;
			life += 1;
			combo = false;
		}
		else {
			boss.life -= 2 * bonusDmg;
			life += 1;
		}
		isBuffed = false;
	}
	else {
		if (combo) {
			boss.life -= 4;
			combo = false;
		}
		else {
			boss.life -= 2;
		}
	}
}
//Incr damage
void Archer::Action3(Boss boss) {
	isBuffed = true;
}

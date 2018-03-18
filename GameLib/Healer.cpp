#include "Healer.h"

Healer::Healer()
{
	maxLife = 80;
	life = maxLife;
}

Healer::~Healer()
{
}

void Healer::Action1(Boss boss) {
	boss.life -= 4;
}

void Healer::Action2(Boss boss) {
	life += 20;
	if (life > maxLife) { life = maxLife; }
}

void Healer::Action3(Boss boss) {
	boss.life -= 2;
	life += 5;
	if (life > maxLife) { life = maxLife; }
}
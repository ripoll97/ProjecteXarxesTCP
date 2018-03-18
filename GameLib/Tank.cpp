#include "Tank.h"

Tank::Tank()
{
	maxLife = 120;
	life = maxLife;
}

Tank::~Tank()
{
}

void Tank::Action1(Boss boss) {
	life += 10;
	if (life > maxLife) { life = maxLife; }
}

void Tank::Action2(Boss boss) {
	boss.life -= 5;
}

void Tank::Action3(Boss boss) {
	boss.life -= 3;
	life += 2;
	if (life > maxLife) { life = maxLife; }
}

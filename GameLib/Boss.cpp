#include "Boss.h"


Boss::Boss()
{
	tankAggro = true;
	maxLife = 100;
	life = maxLife;

	singleDmg = 40;
	doubleDmg = 30;
	multipleDmg = 20;

}

Boss::~Boss()
{
}

//void Boss::Action1(std::vector<Player>aPlayers) {
//		if (tankAggro) {
//			aPlayers[0].life -= singleDmg;
//			tankAggro = false;
//		}
//		else if (!tankAggro) {
//			aPlayers[3].life -= singleDmg;
//			tankAggro = true;
//		}
//}
//void Boss::Action2(std::vector<Player>aPlayers) {
//	if (life > 50) {
//		aPlayers[1].life -= doubleDmg;
//		aPlayers[2].life -= doubleDmg;
//	}
//}
//void Boss::Action3(std::vector<Player>aPlayers) {
//		for (int i = 0; i < 4; i++) {
//			aPlayers[i].life -= multipleDmg;
//		}
//}

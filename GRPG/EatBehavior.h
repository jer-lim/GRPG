#ifndef EATBEHAVIOR_H
#define EATBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class Player;

using namespace std;

class EatBehavior : public Behavior
{
private:
	Entity* food;
	Player* player;
public:
	EatBehavior(Player* p,Entity* e){
		player = p;
		food = e; 
	}
	bool exists(){ return true; }
	string displayText();
	void action();
};
#endif
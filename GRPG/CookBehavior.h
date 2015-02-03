#ifndef COOKBEHAVIOR_H
#define COOKBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class Player;

using namespace std;

class CookBehavior :public Behavior
{
private:
	Game* gamePtr;
	Entity* food;
	Player* player;
public:
	CookBehavior(Game* game,Player* p, Entity* e){
		gamePtr = game;
		player = p;
		food = e;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif
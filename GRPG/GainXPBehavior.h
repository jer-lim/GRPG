#ifndef GAINXPBEHAVIOR_H
#define GAINXPBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class Player;
class UI;

using namespace std;

class GainXPBehavior : public Behavior
{
private:
	Game* gamePtr;
	Entity* item;
	Player* player;
	UI* ui;
public:
	GainXPBehavior(Game* game, Entity* e, Player* p, UI* u){
		gamePtr = game;
		player = p;
		item = e;
		ui = u;
	}
	bool exists(){ return true; }
	string displayText();
	void action();
};
#endif
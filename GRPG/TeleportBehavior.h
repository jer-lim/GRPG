#ifndef _TELEPORT_H
#define _TELEPORT_H

#include "Behavior.h"
#include "entity.h"
using namespace std;
class Player;
class NPC;

class TeleportBehavior : public Behavior
{
private:
	Player* player;
	NPC* npc;
	VECTOR2 coordinates;
public:
	TeleportBehavior(Player* p, NPC* n, VECTOR2 coord){
		player = p;
		npc = n;
		coordinates = coord;
	}
	TeleportBehavior(Player* p, NPC* n, float x, float y){
		player = p;
		npc = n;
		coordinates = VECTOR2(x, y);
	}
	virtual ~TeleportBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};

#endif
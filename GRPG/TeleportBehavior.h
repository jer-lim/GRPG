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
	char tileID;
	MapLoader* ml;
public:
	TeleportBehavior(Player* p, NPC* n, MapLoader* m, char tid){
		player = p;
		npc = n;
		ml = m;
		tileID = tid;
	}
	virtual ~TeleportBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};

#endif
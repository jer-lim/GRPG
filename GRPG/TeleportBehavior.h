//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _TELEPORT_H
#define _TELEPORT_H

#include "Behavior.h"
#include "entity.h"
using namespace std;
class Player;
class NPC;
class Game;

class TeleportBehavior : public Behavior
{
private:
	Player* player;
	//NPC* npc;
	Entity* guyDoingTeleport;
	char tileID;
	MapLoader* ml;
	Game* gameptr;
public:
	TeleportBehavior(Player* p, MapLoader* m, Entity* e, Game* g, char tid){//NPC* n, MapLoader* m, Entity* e, char tid){
		player = p;
		//npc = n;
		ml = m;
		guyDoingTeleport = e;
		gameptr = g;
		tileID = tid;
	}
	virtual ~TeleportBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};

#endif
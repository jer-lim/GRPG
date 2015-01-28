#include "NPC.h"
#include "PersonLoader.h"
#include "drawManager.h"

void NPC::spawn(Game* gamePtr, PersonLoader* pl, int npcId, VECTOR2 coords, Entity* victim){
	Entity* enemy = new Entity();
	enemy->initialize(gamePtr, pl->getNPC(npcId));
	enemy->setX(coords.x);
	enemy->setY(coords.y);
	enemy->setVictim(victim);
	gamePtr->getDrawManager()->addObject(enemy, 1);
}
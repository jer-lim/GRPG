#include "NPC.h"
#include "PersonLoader.h"
#include "drawManager.h"

Entity* NPC::spawn(Game* gamePtr, int npcId, VECTOR2 coords, Entity* victim){
	Entity* enemy = new Entity();
	enemy->initialize(gamePtr, gamePtr->getPersonLoader()->getNPC(npcId));
	enemy->setX(coords.x);
	enemy->setY(coords.y);
	enemy->setVictim(victim);
	gamePtr->getDrawManager()->addObject(enemy, 1);
	return enemy;
}

void NPC::setTalkText(string txt) {
	
	talkText = String_Functions::translateNewLines(txt);
}
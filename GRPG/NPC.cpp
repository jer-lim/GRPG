//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161

#include "NPC.h"
#include "PersonLoader.h"
#include "drawManager.h"
#include "Aidil.h"
#include "grpg.h"

Entity* NPC::spawn(Game* gamePtr, int npcId, VECTOR2 coords, Entity* victim){
	Entity* enemy;
	if (npcId == 14) //Aidil exception
	{
		enemy = new Aidil();
		((Aidil*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), gamePtr->getPersonLoader()->getNPC(npcId));
	}
	else
	{
		enemy = new Entity();
		enemy->initialize(gamePtr, gamePtr->getPersonLoader()->getNPC(npcId));
	}
	enemy->setX(coords.x);
	enemy->setY(coords.y);
	enemy->setVictim(victim);
	gamePtr->getDrawManager()->addObject(enemy, 1);
	return enemy;
}

void NPC::setTalkText(string txt) {
	
	talkText = String_Functions::translateNewLines(txt);
}
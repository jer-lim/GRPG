//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161

#include "MineBehavior.h"
#include "player.h"
#include "UI.h"

void MineBehavior::action(){
	VECTOR2 collisionVector;
	if (mine->collidesWith(*thePlayer, collisionVector))
	{
		//Time to start mining!
		//ui->addChatText("You need a pickaxe to mine!");
		thePlayer->startMining(mine->getX() < thePlayer->getX());
	}
	else
	{
		thePlayer->setVictim(mine);
		thePlayer->setNPCAction(this);
	}
}
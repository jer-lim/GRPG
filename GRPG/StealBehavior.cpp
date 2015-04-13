//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "StealBehavior.h"
#include "NPC.h"
#include "UI.h"

string StealBehavior::displayText(){ return "Talk to " + ii->getname(); }
void StealBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*thePlayer, collisionVector))
	{
		ui->addChatText(ii->getname() + ": " + ii->getTalkText());
	}
	else
	{
		thePlayer->setVictim(entity);
		thePlayer->setNPCAction(this);
	}
}
//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "FishBehavior.h"
#include "player.h"
#include "UI.h"

string FishBehavior::displayText(){ return "Fish"; }
void FishBehavior::action(){
	VECTOR2 collisionVector;
	if (fishes->collidesWith(*thePlayer, collisionVector))
	{
		//Time to start fishing!
		//ui->addChatText("You need a fishing net to fish!");
		thePlayer->startFishing(fishes->getX() < thePlayer->getX());
	}
	else
	{
		thePlayer->setVictim(fishes);
		thePlayer->setNPCAction(this);
	}
}
#include "MineBehavior.h"
#include "player.h"
#include "UI.h"

void MineBehavior::action(){
	VECTOR2 collisionVector;
	if (mine->collidesWith(*thePlayer, collisionVector))
	{
		//Time to start fishing!
		//ui->addChatText("You need a fishing net to fish!");
		thePlayer->startMining(mine->getX() < thePlayer->getX());
	}
	else
	{
		thePlayer->setVictim(mine);
		thePlayer->setNPCAction(this);
	}
}
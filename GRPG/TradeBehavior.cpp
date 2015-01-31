#include "tradeBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"

string TradeBehavior::displayText(){ return "Trade with " + ii->getname(); }
void TradeBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*thePlayer, collisionVector))
	{
		ui->drawWindow("Grocer's Store");
	}
	else
	{
		thePlayer->setVictim(entity);
		thePlayer->setNPCAction(this);
	}
}
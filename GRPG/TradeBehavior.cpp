//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161

#include "tradeBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"

string TradeBehavior::displayText(){ return "Trade with " + ii->getname(); }
void TradeBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*thePlayer, collisionVector))
	{
		ui->drawWindow(ii->getname() + "'s Store");
		ui->setShopItems(entity->getInventory()->getVectorItems());
	}
	else
	{
		thePlayer->setVictim(entity);
		thePlayer->setNPCAction(this);
	}
}
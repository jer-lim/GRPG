#include "StoveMoveBehavior.h"
#include "player.h"
#include "UI.h"

string StoveMoveBehavior::displayText(){
	return "Move to stove";
}

//player attacks enemy, and not anything else (is NOT a replacement for entity::attack)
void StoveMoveBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector))
	{
		player->setNearStove(true);
		ui->addChatText("You are now beside a stove");
	}
	else
	{
		player->setVictim(entity);
		player->setDestination(0);
	}
}
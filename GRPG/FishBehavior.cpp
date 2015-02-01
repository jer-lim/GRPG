#include "FishBehavior.h"
#include "player.h"

string FishBehavior::displayText(){ return "Fish"; }
void FishBehavior::action(){
	VECTOR2 collisionVector;
	if (fishes->collidesWith(*thePlayer, collisionVector))
	{
		//Time to start fishing!
	}
	else
	{
		thePlayer->setVictim(fishes);
		thePlayer->setNPCAction(this);
	}
}
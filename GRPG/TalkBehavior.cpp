#include "talkBehavior.h"
#include "NPC.h"
#include "UI.h"

string TalkBehavior::displayText(){ return "Talk to " + ii->getname(); }
void TalkBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*thePlayer, collisionVector))
	{
		ui->addChatText(ii->getname() + ": Hello! Would you like to trade? ");
		ui->addChatText("(To trade, right click on the " + ii->getname() + ", then select 'Trade with " + ii->getname() + "')");
	}
	else
	{
		thePlayer->setVictim(entity);
		thePlayer->setNPCAction(this);
	}
}
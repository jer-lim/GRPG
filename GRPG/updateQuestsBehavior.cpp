//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "UpdateQuestsBehavior.h"
#include "player.h"
#include "entity.h"
#include "UI.h"
#include "grpg.h"

string UpdateQuestsBehavior::displayText(){
	return ("Update Quests");
}

//Finish quests if needed
void UpdateQuestsBehavior::action()
{/*
 //deprecated
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector))
	{
		bool updated = false;
		map<int, Quest*>* mapQuests = questLoader->getMapQuests();
		//loop through quests
		for (map<int, Quest*>::iterator it = mapQuests->begin(); it != mapQuests->end(); ++it){
			if (it->second->completed())
			{//give reward
				//create gold based on int yeah i'mlazy and out of time sue me
				InventoryItem* x = new InventoryItem(grpg->getItemLoader()->getItem(0), it->second->getgold());
				Entity* newObj = new Entity();
				newObj->initialize(grpg, x, false);

				int result = player->getInventory()->addEntityInventoryItem(newObj, grpg);
				if (result == FAILED || result == PARTIAL_MERGE)
				{
					//drop the rest on the ground
					newObj->setX(player->getX());
					grpg->getDrawManager()->addObject(newObj);
					newObj->setY(player->getY());
				}
				else if (result == MERGED)
				{
					//Matt says it should already be handled
					//delete newObj;
				}

				it->second->reset();//reset that quest has been completed
				ui->addChatText("You have claimed your rewards for a quest!");
				updated = true;
			}
		}
		if (!updated)
		{
			ui->addChatText("You have no rewards to claim.");
		}
	}
	else
	{
		player->setVictim(entity);
		player->setNPCAction(this);
	}*/
}
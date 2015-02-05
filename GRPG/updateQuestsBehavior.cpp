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
{
	map<int, Quest*>* mapQuests = questLoader->getMapQuests();
	//loop through quests
	for (map<int, Quest*>::iterator it = mapQuests->begin(); it != mapQuests->end(); ++it){
		if (it->second->getQuestCondition()->completed())
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

			it->second->getQuestCondition()->reset();//reset that quest has been completed
			ui->addChatText("You have completed a quest!");
		}
	}
}
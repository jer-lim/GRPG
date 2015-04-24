//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "EasterInteractBehavior.h"
#include "player.h"
#include "npc.h"
#include "UI.h"
#include "GameEvent_EntityAction.h"
#include "grpg.h"

string EasterInteractBehavior::displayText(){
	return "Interact with Bird";
}

void EasterInteractBehavior::action()
{
	VECTOR2 collisionVector;
	if (theBird->collidesWith(*player, collisionVector))
	{
		bool actionPerformed = false;
		//If the feather has already been retrieved, so the "required" is false
		if (!questData->getValue("easterFeatherRequired") && !questData->getValue("easterRebuildNestFinished"))
		{
			actionPerformed = true;
			//Use the feather to build up the nest
			ui->addChatText("You use the feather you already have to rebuild the nest");
			gem->informListeners(new GameEvent_EntityAction(npc));
		}
		if (!questData->getValue("foodGiven"))
		{
			//Check if the player's inventory contains the item
			Entity* theItem = new Entity();
			theItem->initialize(grpg, new InventoryFood(grpg->getItemLoader()->getItem(3), 1, FOOD_STATE::COOKED));
			vector<Entity*> entitiesToDelete;
			int size = player->getInventory()->removeEntityInventoryItems(theItem, true, &entitiesToDelete, grpg);
			if (size != -1)
			{
				actionPerformed = true;
				ui->addChatText("You give some food to the bird.");
				gem->informListeners(new GameEvent_ItemUsed(theItem->getInventoryItem()->getItem()));
				//Remove those items
				for (int i = 0; i < size; ++i)
				{
					delete entitiesToDelete.at(i);
					entitiesToDelete[i] = nullptr;
				}
			}
			delete theItem;
		}
		if (!actionPerformed)
		{
			ui->addChatText("There's nothing you can do here at the moment.");
			ui->addChatText("Talk to the easter bunny to see what needs doing.");
		}
	}
	else
	{
		player->setVictim(theBird);
		player->setNPCAction(this);
	}
}
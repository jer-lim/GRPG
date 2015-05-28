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
		if (questData->getValue("easterBirdNestStatus") == 2)
		{
			actionPerformed = true;
			//Use the feather to build up the nest
			ui->addChatText("You use the feather you already have to rebuild the nest");
			gem->informListeners(new GameEvent_EntityAction(npc));
		}
		if (!questData->getValue("foodGiven") && questData->getValue("easterStatus") == 1)
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
			else
			{
				//Try delicious as well
				delete theItem;
				theItem = new Entity();
				theItem->initialize(grpg, new InventoryFood(grpg->getItemLoader()->getItem(3), 1, FOOD_STATE::DELICIOUS));
				vector<Entity*> entitiesToDelete;
				int size = player->getInventory()->removeEntityInventoryItems(theItem, true, &entitiesToDelete, grpg);
				if (size != -1)
				{
					actionPerformed = true;
					ui->addChatText("You give some food to the bird.");
					gem->informListeners(new GameEvent_ItemUsed(theItem->getInventoryItem()->getItem()));
					//Grant 1k bonus cooking xp on quest complete for giving delicious food.
					questData->setValue("easterGoodFoodGiven", 1);
					//Remove those items
					for (int i = 0; i < size; ++i)
					{
						delete entitiesToDelete.at(i);
						entitiesToDelete[i] = nullptr;
					}
				}
			}
			delete theItem;
		}
		if (questData->getValue("easterStatus") == 2)
		{
			actionPerformed = true;
			ui->addChatText("You use soothing words to try and calm the bird...");
			ui->addChatText("You manage to successfully calm the bird.");
			gem->informListeners(new GameEvent_EntityAction(npc));
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
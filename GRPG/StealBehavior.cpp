//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "StealBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "Item.h"
#include "grpg.h"

string StealBehavior::displayText(){ return "Steal from " + ii->getname(); }
void StealBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*thePlayer, collisionVector))
	{
		int stealLevel = ii->getStealLevel();
		int playerStealLevel = thePlayer->getSkills()->at(skillNS::ID_SKILL_THIEVING).getSkillLevel();
		stringstream ss;
		ss << "You need a thieving level of " << stealLevel << " to steal from the " << ii->getname();
		if (playerStealLevel < stealLevel)
		{
			ui->addChatText(ss.str());
		}
		else
		{
			bool success = true;
			int random = rand() % playerStealLevel;
			if (random <= stealLevel)
			{
				if (rand() % 2 == 0)
				{
					success = false;
				}
			}
			if (success)
			{
				vector<Item*>* possibleLoot = ii->getStealItemsList();
				int lootIndex = rand() % possibleLoot->size();
				Item* lootedItem = possibleLoot->at(lootIndex);
				InventoryItem* newLoot = new InventoryItem(lootedItem, 1);
				stringstream ss;
				ss << "You successfully steal ";
				if (lootedItem->getID() == 0)//If coins
				{
					//Give some more
					newLoot->setCurrentStackCount(4);
					ss << "some coins";
				}
				else
				{
					ss << "1" + lootedItem->getName();;
				}
				ui->addChatText(ss.str());

				Entity* newObj = new Entity();
				newObj->initialize(grpg, newLoot, false);
				thePlayer->getInventory()->addEntityInventoryItem(newObj, grpg);
				thePlayer->getSkills()->at(skillNS::ID_SKILL_THIEVING).gainXP();

				//thePlayer->getInventory()->add
			}
			else
			{
				entity->sayMessage("Hey! What's your hand doing in my pocket?");
				//Minimum 2 damage
				((Entity*)thePlayer)->damage(max(2, thePlayer->getHealth() / 10));
			}
		}
	}
	else
	{
		thePlayer->setVictim(entity);
		thePlayer->setNPCAction(this);
	}
}
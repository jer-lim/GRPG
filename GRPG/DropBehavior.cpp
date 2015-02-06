// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "DropBehavior.h"
#include "PickupBehavior.h"
#include "player.h"

void DropBehavior::action(){
	drawManager->addObject(entity, 1);
	player->getInventory()->removeEntityInventoryItem(entity,(Grpg*)gamePtr);//remove from inventory

	//Already deleted inside removeEntityInventoryItem
	//delete this;//THIS MUST BE CALLED IF YOU INTEND TO REMOVE THE BEHAVIOR
}
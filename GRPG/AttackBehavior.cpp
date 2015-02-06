//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "AttackBehavior.h"
#include "NPC.h"
#include "player.h"

string AttackBehavior::displayText(){ 
	return "Attack " + npc->getname();
}

//player attacks enemy, and not anything else (is NOT a replacement for entity::attack)
void AttackBehavior::action()
{
	player->setVictim(entity);
	player->releaseDestination();
	player->setDestination(0);
}
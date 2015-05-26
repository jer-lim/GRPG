//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _RESOURCE_H			         // Prevent multiple definitions if this 
#define _RESOURCE_H		             // file is included in more than one place

//Ignore this file for now, it is never ever used

#include "Tile.h"
#include "FishBehavior.h"
#include "MineBehavior.h"

namespace resourceNS
{
	const int FISHING = 1;
	const int MINING = 2;
}

class Resource : public Tile
{
private:
public:
	Resource(char i);

	virtual ~Resource();

	// Initialize resource by specifying which type it should be
	// Pre: *gamePtr = pointer to Game object
	//		type = which type (from resourceNS) this resource should be
	virtual bool initialize(Game *gamePtr, int type, TextureManager* tm);

	// Overrides from tile so it does nothing
	virtual bool initialize(Game *gamePtr, TextureManager* tm) { return true; }

	// Checks the questData to see if there has been anything updated specifically for this entity
	// and performs any required changes
	// Generally called on a game update.
	virtual void questAction(QuestData* questData, GameEventManager* gem);

	virtual string getType(){ return "RESOURCE"; }

};

#endif
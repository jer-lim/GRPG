//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "Resource.h"
#include "grpg.h"

Resource::Resource(char i) : Tile(i) { };
Resource::~Resource() {};

// Initialize resource by specifying which type it should be
// Pre: *gamePtr = pointer to Game object
//		type = which type (from resourceNS) this resource should be
bool Resource::initialize(Game *gamePtr, int type, TextureManager* tm)
{
	if (type == resourceNS::FISHING)
	{
		fishBehavior = new FishBehavior(((Grpg*)gamePtr)->getPlayer(), this, ((Grpg*)gamePtr)->getUI());
		viewBehavior = new ViewBehavior("Fish", "A school of fishes", ((Grpg*)gamePtr)->getUI());
	}
	else
	{
		mineBehavior = new MineBehavior(((Grpg*)gamePtr)->getPlayer(), this, ((Grpg*)gamePtr)->getUI());
		viewBehavior = new ViewBehavior("Ore", "Some ores", ((Grpg*)gamePtr)->getUI());
	}
	setupVectorActiveBehaviors();
	if (Entity::initialize(gamePtr, tileNS::HEIGHT, tileNS::WIDTH, 0, tm, false))
	{
		edge.top = image.getHeight() / 2;
		edge.bottom = image.getHeight() / 2;
		edge.left = image.getWidth() / 2;
		edge.right = image.getWidth() / 2;
		return true;
	}
	else
	{
		return false;
	}
}
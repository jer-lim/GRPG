//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "Resource.h"
#include "KeyFishBehavior.h"
#include "grpg.h"
#include "mapLoader.h"

Resource::Resource(char i) : Tile(i) { };
Resource::~Resource() {};

// Initialize resource by specifying which type it should be
// Pre: *gamePtr = pointer to Game object
//		type = which type (from resourceNS) this resource should be
bool Resource::initialize(Game *gamePtr, int type, TextureManager* tm)
{
	theGame = gamePtr;
	thePlayer = ((Grpg*)gamePtr)->getPlayer();
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
	//Also, call questAction to ensure that any quest states are done properly
	questAction(((Grpg*)gamePtr)->getQuestLoader()->getQuestData(), ((Grpg*)gamePtr)->getGameEventManager());
	//And add it so that any further updates are processed
	((Grpg*)gamePtr)->getGameEventManager()->addListener(this);
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

void Resource::questAction(QuestData* questData, GameEventManager* gem)
{
	//Am I the special fishing spot at the rich man's house?
	VECTOR2 coords = ((Grpg*)theGame)->getMapLoader()->translateIdToCoords('%');
	if (coords.x == x && coords.y == y)
	{
		SAFE_DELETE(fishBehavior);
		keyFishBehavior = new KeyFishBehavior(thePlayer, this, ((Grpg*)theGame)->getUI(), (Grpg*)theGame);
		setupVectorActiveBehaviors();
	}
}
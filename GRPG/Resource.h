#ifndef _RESOURCE_H			         // Prevent multiple definitions if this 
#define _RESOURCE_H		             // file is included in more than one place

//Ignore this file for now, it is never ever used

#include "entity.h"
#include "FishBehavior.h"
#include "MineBehavior.h"

namespace resourceNS
{
	const int FISHING = 1;
	const int MINING = 2;
}

class Resource : public Entity
{
private:
public:
	Resource();

	virtual ~Resource();

	// Update Entity.
	// typically called once per frame
	// frameTime is used to regulate the speed of movement and animation
	// Left empty in resource to save processing power (No need to call Entity::update())
	virtual void update(float frameTime, Game* gamePtr = nullptr) { };

	// Initialize resource by specifying which type it should be
	// Pre: *gamePtr = pointer to Game object
	//		type = which type (from resourceNS) this resource should be
	virtual bool initialize(Game *gamePtr, int type, TextureManager* tm)
	{
		if (type == resourceNS::FISHING)
		{
			fishBehavior = new FishBehavior();
			viewBehavior = new ViewBehavior("A school of fishes");
		}
		else
		{
			mineBehavior = new MineBehavior();
			viewBehavior = new ViewBehavior("Some ores");
		}
		return Entity::initialize(gamePtr, 0, 0, 0, tm, false);
	}
};

#endif
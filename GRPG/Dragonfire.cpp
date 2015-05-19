#include "Dragonfire.h"
#include "grpg.h"
#include "Aidil.h"

void Dragonfire::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

bool Dragonfire::initialize(Game *gamePtr, TextureManager* tm, Destination* d, float rotation)
{
	theGame = gamePtr;
	bool result = Entity::initialize(gamePtr, 0, 0, 0, tm);
	setDestination(d);
	//I have no idea why I have to do this with the rotation;
	//or why the passed in rotation doesn't work in the first place
	//(When the passed in rotation is the one used to generate the background
	//primitive triangle to warn the player where the dragonbreath will be coming out;
	//and it works just fine for that purpose). But if I don't manipulate the angle
	//in this way it ends up being wrong. Welp; it ends up working anyway.
	float rot = rotation - PI / 2;
	rot = 2 * PI - rot;
	image.setRadians(rot);
	return result;
}

void Dragonfire::update(float frameTime, Game* gamePtr)
{
	thePlayer = ((Grpg*)gamePtr)->getPlayer();
	//Don't bother doing pathfinding with these, we'll do a simple one
	//Entity::update(frameTime, gamePtr);
	//calculate angle
	VECTOR2 direction = destination->getVector() - getVector();
	VECTOR2 *normalizedDirection = &VECTOR2();
	D3DXVec2Normalize(normalizedDirection, &direction);

	setX(getX() + normalizedDirection->x * aidilNS::dragonfireSpeed * frameTime);
	setY(getY() + normalizedDirection->y * aidilNS::dragonfireSpeed * frameTime);

	//Check for collision with player
	VECTOR2 collisionVector;
	if (thePlayer->collidesWith(*this, collisionVector))
	{
		if (!thePlayer->getImmunityToDragonfire())
		{
			int bonusDamage = rand() % (aidilNS::dragonfireMaxDamage - aidilNS::dragonfireMinDamage);
			thePlayer->damage(aidilNS::dragonfireMinDamage + bonusDamage);
			thePlayer->hitByDragonfire();
		}
	}

	float distanceToDest = D3DXVec2Length(&direction);
	if (distanceToDest < aidilNS::dragonfireSpeed * frameTime)
	{
		//We've reached our destination (or are close enough to it), yay!
		gamePtr->setMouseOverEntity(nullptr);
		gamePtr->getDrawManager()->removeObject(this);
		delete this;
	}
}
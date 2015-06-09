#include "Rift.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "RiftEnterBehavior.h"
#include "AnimatableTile.h"
#include "RiftExitBehavior.h"
#include "player.h"

Rift::Rift() : Entity()
{
	riftTexture = new TextureManager();
	currentWave = 0;
}

Rift::~Rift()
{
	SAFE_DELETE(riftTexture);
}

bool Rift::initialize(Game* gamePtr, Player* p, NPC* character)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	person = character;

	if (!riftTexture->initialize(graphics, riftNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize rift texture.");
	}
	bool result = Entity::initialize(gamePtr, riftNS::imageWidth, riftNS::imageHeight, 0, riftTexture);
	setupBehaviors();

	return result;
}

void Rift::draw(Viewport* viewport)
{
	//The rift must always be visible for the player to change between realms,
	//so it must always be drawn and thus be in the same realm as the player
	setIsInDarkRealm(thePlayer->inDarkRealm());
	Entity::draw(viewport);
}

void Rift::update(float frameTime, Game* gamePtr)
{
	//Plant Patch never moves, attacks ... or get attacked, so no use for entity update
	//Entity::update(frameTime, gamePtr);
	//Rotate accordingly
	image.setDegrees(image.getDegrees() + (frameTime * 360) / riftNS::timeForFullRotation);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void Rift::onLostDevice()
{
	riftTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Rift::onResetDevice()
{
	riftTexture->onResetDevice();
}

void Rift::setupBehaviors()
{
	SAFE_DELETE(enterBehavior);
	SAFE_DELETE(exitBehavior);
	SAFE_DELETE(viewBehavior);
	if (thePlayer->inDarkRealm())
	{
		exitBehavior = new RiftExitBehavior(thePlayer, this, ui, (Grpg*)theGame);
	}
	else
	{
		enterBehavior = new RiftEnterBehavior(thePlayer, this, ui, (Grpg*)theGame);
	}
	viewBehavior = new ViewBehavior("Rift", ((NPC*)person)->getdescription(), ui);
	setupVectorActiveBehaviors();
}
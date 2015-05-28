#include "ShriveledMan.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "TalkBehavior.h"

ShriveledMan::ShriveledMan() : Entity()
{
	manTexture = new TextureManager();
}

ShriveledMan::~ShriveledMan()
{
	SAFE_DELETE(manTexture);
}

bool ShriveledMan::initialize(Game* gamePtr, Player* p, Destination* location, NPC* npc)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	person = npc;

	if (!manTexture->initialize(graphics, shriveledManNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize block rock texture.");
	}
	bool result = Entity::initialize(gamePtr, shriveledManNS::imageWidth, shriveledManNS::imageHeight, 0, manTexture);
	talkBehavior = new TalkBehavior(npc, ui, thePlayer, this, (Grpg*)gamePtr);
	viewBehavior = new ViewBehavior("Shriveled Man", npc->getdescription(), ui);
	setupVectorActiveBehaviors();
	setX(location->getX());
	setY(location->getY());

	return result;
}

void ShriveledMan::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

void ShriveledMan::update(float frameTime, Game* gamePtr)
{
	Entity::update(frameTime, gamePtr);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void ShriveledMan::onLostDevice()
{
	manTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void ShriveledMan::onResetDevice()
{
	manTexture->onResetDevice();
}
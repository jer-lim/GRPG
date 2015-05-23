#include "RichDoor.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "OpenBehavior.h"
#include "PickLockBehavior.h"

RichDoor::RichDoor() : Entity()
{
	doorTexture = new TextureManager();
}

RichDoor::~RichDoor()
{
	_ASSERTE(_CrtCheckMemory());
	SAFE_DELETE(doorTexture);
	SAFE_DELETE(openBehavior);
	SAFE_DELETE(pickLockBehavior);
}

bool RichDoor::initialize(Game* gamePtr, Player* p, Destination* location, string examineText)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;

	if (!doorTexture->initialize(graphics, richDoorNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize rich door texture.");
	}
	Entity::initialize(gamePtr, richDoorNS::imageWidth, richDoorNS::imageHeight, 0, doorTexture);
	viewBehavior = new ViewBehavior("Door", examineText, ui);
	openBehavior = new OpenBehavior(thePlayer, this, ui, ((Grpg*)gamePtr)->getQuestLoader()->getQuestData());
	pickLockBehavior = new PickLockBehavior(thePlayer, this, ui);
	setupVectorActiveBehaviors();

	setX(location->getX());
	setY(location->getY());
}

void RichDoor::draw(Viewport* viewport)
{
	if (open)
	{
		return;
	}

	Entity::draw(viewport);
}

void RichDoor::update(float frameTime, Game* gamePtr)
{
	if (open)
	{
		return;
	}

	//Door never moves, attacks ... or get attacked, ... or do anything, really.
	//Entity::update(frameTime, gamePtr);

	//Check for collision with player
	VECTOR2 collisionVector;
	if (thePlayer->collidesWith(*this, collisionVector))
	{
		//Nope, no crossing.
		if (thePlayer->getY() > getY())
		{
			thePlayer->setY(getY() + richDoorNS::imageHeight / 2 + richDoorNS::collideMargin);
		}
		else
		{
			thePlayer->setY(getY() - richDoorNS::imageHeight / 2 - richDoorNS::collideMargin);
		}
		thePlayer->releaseDestination();
		thePlayer->setVictim(nullptr);
		ui->addChatText("The door is closed, so you can't go through it.");

	}
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void RichDoor::onLostDevice()
{
	doorTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void RichDoor::onResetDevice()
{
	doorTexture->onResetDevice();
}
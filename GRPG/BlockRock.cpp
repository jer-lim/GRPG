#include "BlockRock.h"
#include "Aidil.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "AnimatableTile.h"
#include "BlockRockMineBehavior.h"

BlockRock::BlockRock() : Entity()
{
	_ASSERTE(_CrtCheckMemory());

	rockTexture = new TextureManager();

	_ASSERTE(_CrtCheckMemory());
	firstMine = true;
}

BlockRock::~BlockRock()
{
	_ASSERTE(_CrtCheckMemory());
	SAFE_DELETE(rockTexture);
	SAFE_DELETE(blockRockMineBehavior);
}

bool BlockRock::initialize(Game* gamePtr, Player* p, Destination* location, string et)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	examineText = et;

	if (!rockTexture->initialize(graphics, blockRockNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize block rock texture.");
	}
	Entity::initialize(gamePtr, blockRockNS::imageWidth, blockRockNS::imageHeight, 0, rockTexture);
	setupBehaviors();
	setX(location->getX());
	setY(location->getY());
}

void BlockRock::draw(Viewport* viewport)
{
	if (disabled)
	{
		return;
	}

	Entity::draw(viewport);
}

void BlockRock::update(float frameTime, Game* gamePtr)
{
	if (disabled)
	{
		return;
	}

	//Rock never moves, attacks ... or get attacked, ... or do anything, really.
	//Entity::update(frameTime, gamePtr);
	
	//Do the falling thing, if required
	if (destination != nullptr)
	{
		fallSpeed += blockRockNS::fallSpeed * frameTime;
		setY(getY() + fallSpeed * frameTime);
		//Fall till where you were before.
		if (getY() > destination->getY())
		{
			setY(destination->getY());
			releaseDestination();
		}
	}

	//Check for collision with player
	VECTOR2 collisionVector;
	if (thePlayer->collidesWith(*this, collisionVector))
	{
		//Nope, no crossing.
		if (thePlayer->getY() > getY())
		{
			thePlayer->setY(getY() + blockRockNS::imageHeight/2 + blockRockNS::collideMargin);
		}
		else
		{
			thePlayer->setY(getY() - blockRockNS::imageHeight/2 - blockRockNS::collideMargin);
		}
		thePlayer->releaseDestination();
		thePlayer->setVictim(nullptr);
		ui->addChatText("You can't seem to find a way around this rock");
		
	}
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void BlockRock::onLostDevice()
{
	rockTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void BlockRock::onResetDevice()
{
	rockTexture->onResetDevice();
}

void BlockRock::startFall(Viewport* vp)
{
	destination = new Point(getX(), getY());
	setY(vp->getTopLeft().y);
	fallSpeed = 0;
}

void BlockRock::setupBehaviors()
{
	viewBehavior = new ViewBehavior("A rock", examineText, ui);
	blockRockMineBehavior = new BlockRockMineBehavior(thePlayer, this, ui);
	setupVectorActiveBehaviors();
}
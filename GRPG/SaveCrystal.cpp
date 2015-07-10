#include "SaveCrystal.h"
#include "SaveCrystalUseBehavior.h"
#include "Aidil.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"

SaveCrystal::SaveCrystal() : Entity()
{
	saveCrystalTexture = new TextureManager();
}

SaveCrystal::~SaveCrystal()
{
	SAFE_DELETE(saveCrystalTexture);
	SAFE_DELETE(useBehavior);
}

bool SaveCrystal::initialize(Game* gamePtr, Player* p, Destination* location, string et)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	examineText = et;

	if (!saveCrystalTexture->initialize(graphics, saveCrystalNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize save crystal texture.");
	}
	bool result = Entity::initialize(gamePtr, saveCrystalNS::imageWidth, saveCrystalNS::imageHeight, saveCrystalNS::frames, saveCrystalTexture);
	image.setFrameDelay(0.3f);
	image.setFrames(0, saveCrystalNS::frames - 1);
	setupBehaviors();
	setX(location->getX());
	setY(location->getY());

	return result;
}

void SaveCrystal::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

void SaveCrystal::update(float frameTime, Game* gamePtr)
{
	//Rock never moves, attacks ... or get attacked, ... or do anything, really.
	//Entity::update(frameTime, gamePtr);
	image.update(frameTime);

	//Check for collision with player
	/*
	VECTOR2 collisionVector;
	if (thePlayer->collidesWith(*this, collisionVector))
	{
		//Nope, no crossing.
		if (thePlayer->getY() > getY())
		{
			thePlayer->setY(getY() + blockRockNS::imageHeight / 2 + blockRockNS::collideMargin);
		}
		else
		{
			thePlayer->setY(getY() - blockRockNS::imageHeight / 2 - blockRockNS::collideMargin);
		}
		thePlayer->releaseDestination();
		thePlayer->setVictim(nullptr);
		ui->addChatText("You can't seem to find a way around this rock");
	}*/
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void SaveCrystal::onLostDevice()
{
	saveCrystalTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void SaveCrystal::onResetDevice()
{
	saveCrystalTexture->onResetDevice();
}

void SaveCrystal::setupBehaviors()
{
	viewBehavior = new ViewBehavior("Save Crystal", examineText, ui);
	useBehavior = new SaveCrystalUseBehavior(thePlayer, this, ui, (Grpg*) theGame);
	setupVectorActiveBehaviors();
}
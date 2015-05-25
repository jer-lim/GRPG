#include "ArtifactDisplay.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "TakeArtifactBehavior.h"
#include "GameEvent_ItemReceived.h"

ArtifactDisplay::ArtifactDisplay() : Entity()
{
	artifactTexture = new TextureManager();
}

ArtifactDisplay::~ArtifactDisplay()
{
	_ASSERTE(_CrtCheckMemory());
	SAFE_DELETE(artifactTexture);
	SAFE_DELETE(takeArtifactBehavior);
}

bool ArtifactDisplay::initialize(Game* gamePtr, Player* p, Destination* location, string examineText)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	theGame = gamePtr;

	if (!artifactTexture->initialize(graphics, artifactDisplayNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize rich door texture.");
	}
	bool result = Entity::initialize(gamePtr, artifactDisplayNS::imageWidth, artifactDisplayNS::imageHeight, artifactDisplayNS::imageCols, artifactTexture);
	viewBehavior = new ViewBehavior("Artifact Display case", examineText, ui);
	takeArtifactBehavior = new TakeArtifactBehavior(thePlayer, this, ui, ((Grpg*)theGame)->getQuestLoader()->getQuestData(), (Grpg*)theGame);
	setupVectorActiveBehaviors();

	setX(location->getX());
	setY(location->getY());

	return result;
}

void ArtifactDisplay::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

void ArtifactDisplay::update(float frameTime, Game* gamePtr)
{
	//Artifact display never moves, attacks ... or get attacked, ... or do anything, really.
	//Entity::update(frameTime, gamePtr);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void ArtifactDisplay::onLostDevice()
{
	artifactTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void ArtifactDisplay::onResetDevice()
{
	artifactTexture->onResetDevice();
}

void ArtifactDisplay::setOpen(bool o)
{
	open = o;
	if (open)
	{
		if (image.getCurrentFrame() == 0)
		{
			image.setCurrentFrame(1);
		}
		else if (image.getCurrentFrame() == 3)
		{
			image.setCurrentFrame(2);
		}
	}
	else
	{
		if (image.getCurrentFrame() == 1)
		{
			image.setCurrentFrame(0);
		}
		else if (image.getCurrentFrame() == 2)
		{
			image.setCurrentFrame(3);
		}
	}
}
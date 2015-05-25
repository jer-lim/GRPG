#include "ArtifactLever.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "ArtifactDisplay.h"
#include "QuestData.h"
#include "TakeArtifactBehavior.h"
#include "PullArtifactLever.h"
#include "GameEvent_ItemReceived.h"

ArtifactLever::ArtifactLever() : Entity()
{
	leverTexture = new TextureManager();
}

ArtifactLever::~ArtifactLever()
{
	_ASSERTE(_CrtCheckMemory());
	SAFE_DELETE(leverTexture);
	SAFE_DELETE(pullLeverBehavior);
}

bool ArtifactLever::initialize(Game* gamePtr, Player* p, Destination* location, string examineText)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	theGame = gamePtr;

	if (!leverTexture->initialize(graphics, leverDisplayNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize artifact lever texture.");
	}
	bool result = Entity::initialize(gamePtr, leverDisplayNS::imageWidth, leverDisplayNS::imageHeight, leverDisplayNS::imageCols, leverTexture);
	viewBehavior = new ViewBehavior("A Lever", examineText, ui);
	pullLeverBehavior = new PullArtifactLeverBehavior(thePlayer, this, ui, ((Grpg*)theGame)->getQuestLoader()->getQuestData(), (Grpg*)theGame);
	setupVectorActiveBehaviors();

	setX(location->getX());
	setY(location->getY());

	return result;
}

void ArtifactLever::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

void ArtifactLever::update(float frameTime, Game* gamePtr)
{
	//Artifact display never moves, attacks ... or get attacked, ... or do anything, really.
	//Entity::update(frameTime, gamePtr);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void ArtifactLever::onLostDevice()
{
	leverTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void ArtifactLever::onResetDevice()
{
	leverTexture->onResetDevice();
}

void ArtifactLever::setPulled(bool p)
{
	pulled = p;
	stringstream ss;
	VECTOR2 artifactDisplayLocation = ((Grpg*)theGame)->getMapLoader()->translateIdToCoords('#');
	ss << artifactDisplayLocation.x << "," << artifactDisplayLocation.y;
	ArtifactDisplay* ad = (ArtifactDisplay*)((Grpg*)theGame)->getSpawnLink(ss.str());
	if (!pulled)
	{
		image.setCurrentFrame(0);
		((Grpg*)theGame)->getQuestLoader()->getQuestData()->setValue("artifactDisplayStatus", 0);
		ad->setOpen(false);
	}
	else
	{
		image.setCurrentFrame(1);
		((Grpg*)theGame)->getQuestLoader()->getQuestData()->setValue("artifactDisplayStatus", 1);
		ad->setOpen(true);
	}
}
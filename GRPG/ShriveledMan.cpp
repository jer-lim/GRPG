#include "ShriveledMan.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "TalkBehavior.h"

ShriveledMan::ShriveledMan() : Entity()
{
	manTexture = new TextureManager();
	StolenArtifactRunPhrase sarp;
	sarp.phrase = "AHAHAHAHA";
	sarp.time = 1;
	StolenArtifactRunPhrase sarp2; 
	sarp2.phrase = "I GOT THE ARTIFACT";
	sarp.time = 1;
	stolenArtifactRunPhrases.push_back(sarp);
	stolenArtifactRunPhrases.push_back(sarp2);
	stolenArtifactEndRunStatus = -1;
	disappearWhenOutOfView = false;
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
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Shrieveled Man texture.");
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
	if (getX() + shriveledManNS::imageWidth < viewport->getTopLeft().x || getX() - shriveledManNS::imageWidth > viewport->getBottomRight().x ||
		getY() + shriveledManNS::imageHeight < viewport->getTopLeft().y || getY() - shriveledManNS::imageHeight > viewport->getBottomRight().y)
	{
		currentlyVisible = false;
	}
	else
	{
		currentlyVisible = true;
	}
}

void ShriveledMan::update(float frameTime, Game* gamePtr)
{
	Entity::update(frameTime, gamePtr);
	if (stolenArtifactEndRunStatus != -1 && !currentlyVisible)
	{
		releaseDestination();
		stolenArtifactEndRunStatus = -1;
		stolenArtifactTimer = 0;
		//End the quest for the player
		thePlayer->releaseDestination();
		thePlayer->setVictim(nullptr);
		((Grpg*)theGame)->attemptQuestCompletions();
	}
	else if (stolenArtifactEndRunStatus != -1)
	{
		stolenArtifactTimer -= frameTime;
		if (stolenArtifactTimer <= 0)
		{
			stolenArtifactEndRunStatus++;
			if (stolenArtifactEndRunStatus < stolenArtifactRunPhrases.size())
			{
				StolenArtifactRunPhrase sarp = stolenArtifactRunPhrases[stolenArtifactEndRunStatus];
				sayMessage(sarp.phrase);
				stolenArtifactTimer += sarp.time;
			}
			else
			{
				//No more messages to display
				stolenArtifactTimer = 999;
			}
		}
	}
	if (disappearWhenOutOfView && !currentlyVisible)
	{
		fakeDelete();
		disappearWhenOutOfView = false;
	}
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

void ShriveledMan::startStolenArtifactRun()
{
	destination = new Point(((Grpg*)theGame)->getMapLoader()->translateIdToCoords(shriveledManNS::locationToRunTo));
	stolenArtifactEndRunStatus = 0;
	StolenArtifactRunPhrase sarp = stolenArtifactRunPhrases[0];
	stolenArtifactTimer = sarp.time;
	sayMessage(sarp.phrase);
	disappearWhenOutOfView = true;
}

void ShriveledMan::startRiftIntro()
{
	SAFE_DELETE(talkBehavior);
	SAFE_DELETE(viewBehavior);
	sayMessage("Oh, it's you! Welcome, hero!");
	thePlayer->sayMessage("You again!");
	setIsInDarkRealm(true);
	image.setVisible(true);
	setActive(true);
	talkBehavior = new TalkBehavior((NPC*)person, ui, thePlayer, this, (Grpg*)theGame);
	viewBehavior = new ViewBehavior("Shriveled Man", ((NPC*)person)->getdescription(), ui);
	setupVectorActiveBehaviors();
}

void ShriveledMan::continueRiftIntro()
{
	sayMessage("Oh, you'll find out!");
	ui->addChatText("You hear Alfred shout from outside the portal!");
	Rift* theRift = thePlayer->getRiftPortal();
	theRift->sayMessage("Quick! Escape before you're overwhelmed!");
	theRift->begin(true);
	person->setMovementSpeed(thePlayer->getPerson()->getMovementSpeed());
	disappearWhenOutOfView = true;
	Point* newDestination = new Point(getX(), getY() - GAME_HEIGHT*2);
	setDestination(newDestination);
}
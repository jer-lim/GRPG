#include "PlantPatch.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "AnimatableTile.h"
#include "PlantPatchInspectBehavior.h"
#include "PlantPatchRakeBehavior.h"
#include "PlantPatchDigBehavior.h"
#include "PlantPatchPlantBehavior.h"

PlantPatch::PlantPatch() : Entity()
{
	plantPatchTexture = new TextureManager();
}

PlantPatch::~PlantPatch()
{
	SAFE_DELETE(plantPatchTexture);
}

bool PlantPatch::initialize(Game* gamePtr, Player* p, Destination* location, string et)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	examineText = et;

	if (!plantPatchTexture->initialize(graphics, plantPatchNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize plant patch texture.");
	}
	bool result = Entity::initialize(gamePtr, plantPatchNS::imageWidth, plantPatchNS::imageHeight, plantPatchNS::imageCols, plantPatchTexture);
	setupBehaviors();
	setX(location->getX());
	setY(location->getY());

	return result;
}

void PlantPatch::setStatus(int s)
{
	status = s;
	image.setCurrentFrame(s);
}

void PlantPatch::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

void PlantPatch::update(float frameTime, Game* gamePtr)
{
	//Plant Patch never moves, attacks ... or get attacked, ... or do anything, really.
	//Entity::update(frameTime, gamePtr);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void PlantPatch::onLostDevice()
{
	plantPatchTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void PlantPatch::onResetDevice()
{
	plantPatchTexture->onResetDevice();
}

void PlantPatch::setupBehaviors()
{
	inspectBehavior = new PlantPatchInspectBehavior(thePlayer, this, ui, ((Grpg*)theGame)->getQuestLoader()->getQuestData());
	rakeBehavior = new PlantPatchRakeBehavior(thePlayer, this, ui, ((Grpg*)theGame)->getQuestLoader()->getQuestData());
	digBehavior = new PlantPatchDigBehavior(thePlayer, this, ui, ((Grpg*)theGame)->getQuestLoader()->getQuestData());
	plantBehavior = new PlantPatchPlantBehavior(thePlayer, this, ui, ((Grpg*)theGame)->getQuestLoader()->getQuestData());
	viewBehavior = new ViewBehavior("Plant Patch", examineText, ui);
	setupVectorActiveBehaviors();
}
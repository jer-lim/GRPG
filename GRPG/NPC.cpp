//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161

#include "NPC.h"
#include "PersonLoader.h"
#include "drawManager.h"
#include "Aidil.h"
#include "grpg.h"
#include "BlockRock.h"
#include "RichDoor.h"
#include "ArtifactDisplay.h"
#include "ArtifactLever.h"
#include "ShriveledMan.h"
#include "PlantPatch.h"
#include "Rift.h"

Entity* NPC::spawn(Game* gamePtr, int npcId, VECTOR2 coords, Entity* victim){
	Entity* enemy;
	if (npcId == 14) //Aidil exception
	{
		enemy = new Aidil();
		((Aidil*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), gamePtr->getPersonLoader()->getNPC(npcId));
	}
	else if (npcId == 30) //Blockrock exception (I know, not really an npc...)
	{
		_ASSERTE(_CrtCheckMemory());
		enemy = new BlockRock();
		((BlockRock*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), new Point(coords.x, coords.y), gamePtr->getPersonLoader()->getNPC(npcId)->getdescription());
	}
	else if (npcId == 32)
	{
		enemy = new ShriveledMan();
		((ShriveledMan*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), new Point(coords.x, coords.y), gamePtr->getPersonLoader()->getNPC(npcId));
	}
	else if (npcId == 34)
	{
		enemy = new RichDoor();
		((RichDoor*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), new Point(coords.x, coords.y), gamePtr->getPersonLoader()->getNPC(npcId)->getdescription());
	}
	else if (npcId == 35)
	{
		enemy = new ArtifactDisplay();
		((ArtifactDisplay*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), new Point(coords.x, coords.y), gamePtr->getPersonLoader()->getNPC(npcId)->getdescription());
	}
	else if (npcId == 36)
	{
		enemy = new ArtifactLever();
		((ArtifactLever*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), new Point(coords.x, coords.y), gamePtr->getPersonLoader()->getNPC(npcId)->getdescription());
	}
	else if (npcId == 38)
	{
		enemy = new PlantPatch();
		((PlantPatch*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), new Point(coords.x, coords.y), gamePtr->getPersonLoader()->getNPC(npcId)->getdescription());
	}
	else if (npcId == 39)
	{
		enemy = new Rift();;
		((Rift*)enemy)->initialize(gamePtr, ((Grpg*)gamePtr)->getPlayer(), gamePtr->getPersonLoader()->getNPC(npcId));
	}
	else
	{
		enemy = new Entity();
		enemy->initialize(gamePtr, gamePtr->getPersonLoader()->getNPC(npcId));
	}
	enemy->setX(coords.x);
	enemy->setY(coords.y);
	enemy->setVictim(victim);
	gamePtr->getDrawManager()->addObject(enemy, 1);
	return enemy;
}

void NPC::setTalkText(string txt) {
	
	talkText = String_Functions::translateNewLines(txt);
}
#include "Spawner.h"
#include "NPC.h"
#include "PersonLoader.h"
#include <sstream>

Spawner::Spawner(Game* gp, int spawn, int cd, Entity* v) : Tile(){
	gamePtr = gp;
	npcId = spawn;
	cooldown = cd;
	victim = v;
	lastSpawnedTime.QuadPart = 0;
	Tile(false);
}

void Spawner::spawn(){
	stringstream ss;
	ss << getX() << "," << getY();
	if (gamePtr->getSpawnLink(ss.str()) == NULL){
		spawnedNPC = NPC::spawn(gamePtr, npcId, VECTOR2(x, y), victim);
		gamePtr->addSpawnLink(ss.str(), spawnedNPC);
		QueryPerformanceCounter(&lastSpawnedTime);
	}
}

// Can't do respawning yet because NPC can't die yet
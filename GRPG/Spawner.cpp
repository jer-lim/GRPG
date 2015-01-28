#include "Spawner.h"
#include "NPC.h"
#include "PersonLoader.h"

Spawner::Spawner(Game* gp, int spawn, int cd, Entity* v) : Tile(){
	gamePtr = gp;
	npcId = spawn;
	cooldown = cd;
	victim = v;
	Tile(false);
}

void Spawner::spawn(){
	spawnedNPC = NPC::spawn(gamePtr, npcId, VECTOR2(x, y), victim);
	QueryPerformanceCounter(&lastSpawnedTime);
}

// Can't do respawning yet because NPC can't die yet
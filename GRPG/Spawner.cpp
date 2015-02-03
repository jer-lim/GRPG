#include "Spawner.h"
#include "NPC.h"
#include "PersonLoader.h"
#include <sstream>

Spawner::Spawner(char i, Game* gp, int spawn, int cd, Entity* v) : Tile(i){
	gamePtr = gp;
	npcId = spawn;
	cooldown = cd;
	victim = v;
	timeOfDeath.QuadPart = 0;
	collisionType = entityNS::NONE;
	Tile(false);
}

void Spawner::spawn(){
	stringstream ss;
	ss << getX() << "," << getY();
	if (gamePtr->getSpawnLink(ss.str()) == NULL){
		Entity* spawnedNPC = NPC::spawn(gamePtr, npcId, VECTOR2(x, y), victim);
		spawnedNPC->setSpawnPoint(VECTOR2(x,y));
		gamePtr->addSpawnLink(ss.str(), spawnedNPC);
		timeOfDeath.QuadPart = 0; // 0 = no death
	}
}

void Spawner::update(){
	stringstream ss;
	ss << getX() << "," << getY();
	if (gamePtr->getSpawnLink(ss.str()) == NULL && timeOfDeath.QuadPart == 0){ // If dead and not recorded dead yet
		QueryPerformanceCounter(&timeOfDeath); // Record time of death
	}

	if (timeOfDeath.QuadPart != 0){
		LARGE_INTEGER currTime, timerFreq;
		float timeSinceLastSpawn;
		QueryPerformanceCounter(&currTime);
		QueryPerformanceFrequency(&timerFreq);
		timeSinceLastSpawn = (float)(currTime.QuadPart - timeOfDeath.QuadPart) / (float)timerFreq.QuadPart;

		if (timeSinceLastSpawn > cooldown){
			spawn();
		}
	}
}

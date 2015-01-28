#ifndef SPAWNER_H
#define SPAWNER_H

#include "tile.h"

class Spawner : public Tile {
protected:

	Game* gamePtr;
	Entity* victim;
	Entity* spawnedNPC;

	int npcId;
	int cooldown;
	LARGE_INTEGER lastSpawnedTime;
public:
	Spawner(Game* gp, int spawn, int cd, Entity* v);

	void spawn();
};

#endif
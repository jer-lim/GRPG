#ifndef SPAWNER_H
#define SPAWNER_H

#include "tile.h"

class Spawner : public Tile {
protected:
	int spawnedNPC;
	int cooldown;
public:
	Spawner(int spawn, int cd);
};

#endif
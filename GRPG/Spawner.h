#ifndef SPAWNER_H
#define SPAWNER_H

#include "tile.h"

class Spawner : public Tile {
protected:

	Game* gamePtr;
	Entity* victim;

	int npcId;
	int cooldown;
	LARGE_INTEGER timeOfDeath;
public:
	Spawner(Game* gp, int spawn, int cd, Entity* v);

	void spawn();
	virtual void update(); // Handles respawning
};

#endif
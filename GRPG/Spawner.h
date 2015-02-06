// Module: Games Programming
// Assignment 2: GRPG
// Student Name: Jeremy Lim
// Student Number: S10122326F

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
	Spawner(char i, Game* gp, int spawn, int cd, Entity* v);

	void spawn();
	virtual void update(); // Handles respawning
};

#endif
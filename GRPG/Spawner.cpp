#include "Spawner.h"

Spawner::Spawner(int spawn, int cd) : Tile(){
	spawnedNPC = spawn;
	cooldown = cd;
	Tile(false); 
}
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"
#include <map>

class EntityManager {
private:
	map<int, Entity*> entities;
public:

	EntityManager();

	// Functions to handle the menial tasks
	void updateAll(float frameTime);
	void renderAll();
	void releaseAll();
	void resetAll();

	// Add an entity to be managed by the manager
	void addEntity(Entity* ent);
};

#endif
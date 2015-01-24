#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"
#include <map>

class EntityManager {
private:
	map<int, Entity*> entities;
public:

	EntityManager();

	void updateAll(float frameTime);
	void renderAll();
	void releaseAll();
	void resetAll();

	void addEntity(Entity* ent);
};

#endif
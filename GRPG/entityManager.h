#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"
#include <map>

struct ManagedObject {
	int zindex = 1;
	Entity* entity = nullptr;
	Image* image = nullptr;
};

class EntityManager {
private:
	map<int, ManagedObject*> objects;

	void addManagedObject(ManagedObject* mo);

public:

	EntityManager();

	// Functions to handle the menial tasks
	void updateAll(float frameTime);
	void renderAll();
	void releaseAll();
	void resetAll();

	// Add an entity to be managed by the manager
	void addEntity(Entity* ent, int zi = 1);

	void addImage(Image* img, int zi = 1);
};

#endif
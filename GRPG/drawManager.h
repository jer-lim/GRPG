#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include "entity.h"
#include <map>

struct ManagedObject {
	int zindex = 1;
	Entity* entity = nullptr;
	Image* image = nullptr;
};

class DrawManager {
private:
	map<int, ManagedObject*> objects;

	void addManagedObject(ManagedObject* mo);

public:

	DrawManager();

	// Functions to handle the menial tasks
	void updateAll(float frameTime);
	void renderAll();
	void releaseAll();
	void resetAll();

	// Add an entity to be managed by the manager
	void addObject(Entity* ent, int zi = 1);
	void addObject(Image* img, int zi = 1);
};

#endif
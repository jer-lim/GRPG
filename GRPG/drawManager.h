#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <map>
#include "image.h"
#include "grpg.h"

class Entity;
class Viewport;

struct ManagedObject {
	int zindex = 1;
	Entity* entity = nullptr;
	Image* image = nullptr;
};

class DrawManager {
private:
	map<int, ManagedObject*> objects;
	Viewport* viewport;
	Grpg* gamePtr;

	void addManagedObject(ManagedObject* mo);

public:

	DrawManager();
	void initialize(Grpg* gamPtr, Viewport* vp);

	// Functions to handle the menial tasks
	void updateAll(float frameTime);
	void renderAll();
	void releaseAll();
	void resetAll();

	// Add an entity to be managed by the manager
	void addObject(Entity* ent, int zi = 1);
	void addObject(Image* img, int zi = 1);

	void removeObject(Entity* ent);
	void removeObject(Image* img);
};

#endif
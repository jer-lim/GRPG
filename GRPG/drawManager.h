// Module: Games Programming
// Assignment 2: GRPG
// Student Name: Jeremy Lim
// Student Number: S10122326F

#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <map>
#include "image.h"
#include "game.h"

class Entity;
class Viewport;

struct ManagedObject {
	Entity* entity = nullptr;
	Image* image = nullptr;
};

class DrawManager {
private:
	map<int, map<int, ManagedObject*>> objects;
	Viewport* viewport;
	Game* gamePtr;

	void addManagedObject(int zi, ManagedObject* mo);

public:

	DrawManager();
	~DrawManager();
	void initialize(Game* gamPtr, Viewport* vp);

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

	map<int, map<int, ManagedObject*>> getDrawnObjects() { return objects; }
};

#endif
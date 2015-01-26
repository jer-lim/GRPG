#include "drawManager.h"

using namespace std;

DrawManager::DrawManager(){

}

void DrawManager::updateAll(float frameTime){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->update(frameTime);
		else objects[i]->image->update(frameTime);
	}
}

void DrawManager::renderAll(){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->draw();
		else objects[i]->image->draw();
	}
}

void DrawManager::releaseAll(){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->getTextureManager()->onLostDevice();
		else objects[i]->image->getTextureManager()->onLostDevice();
	}
}

void DrawManager::resetAll(){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->getTextureManager()->onResetDevice();
		else objects[i]->image->getTextureManager()->onResetDevice();
	}
}

void DrawManager::addObject(Entity* ent, int zi){
	ManagedObject* mo = new ManagedObject();
	mo->entity = ent;
	mo->zindex = zi;
	
	addManagedObject(mo);
}

void DrawManager::addObject(Image* img, int zi){
	ManagedObject* mo = new ManagedObject();
	mo->image = img;
	mo->zindex = zi;

	addManagedObject(mo);
}

void DrawManager::addManagedObject(ManagedObject* mo){
	bool added = false;
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->zindex > mo->zindex){
			for (int j = objects.size() - 1; j >= i; --j){
				objects[j + 1] = objects[j];
			}
			objects[i] = mo;
			added = true;
			break;
		}
	}

	if (!added){
		objects[objects.size()] = mo;
	}
}
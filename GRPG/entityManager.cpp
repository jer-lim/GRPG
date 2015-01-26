#include "entityManager.h"

using namespace std;

EntityManager::EntityManager(){

}

void EntityManager::updateAll(float frameTime){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->update(frameTime);
		else objects[i]->image->update(frameTime);
	}
}

void EntityManager::renderAll(){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->draw();
		else objects[i]->image->draw();
	}
}

void EntityManager::releaseAll(){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->getTextureManager()->onLostDevice();
		else objects[i]->image->getTextureManager()->onLostDevice();
	}
}

void EntityManager::resetAll(){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity != nullptr) objects[i]->entity->getTextureManager()->onResetDevice();
		else objects[i]->image->getTextureManager()->onResetDevice();
	}
}

void EntityManager::addEntity(Entity* ent, int zi){
	ManagedObject* mo = new ManagedObject();
	mo->entity = ent;
	mo->zindex = zi;
	
	bool added = false;
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->zindex > zi){
			for (int j = objects.size() - 1; j >= i; --j){
				objects[j + 1] = objects[j];
			}
			objects[i] = mo;
			runtimeLog << "Added entity with zindex " << zi << " to " << i << endl;
			added = true;
			break;
		}
	}

	if (!added){
		runtimeLog << "Added entity with zindex " << zi << " to " << objects.size() << endl;
		objects[objects.size()] = mo;
	}
}

void EntityManager::addImage(Image* img, int zi){
	ManagedObject* mo = new ManagedObject();
	mo->image = img;
	mo->zindex = zi;

	bool added = false;
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->zindex > zi){
			for (int j = objects.size() - 1; j >= i; --j){
				objects[j + 1] = objects[j];
			}
			objects[i] = mo;
			runtimeLog << "Added image with zindex " << zi << " to " << i << endl;
			added = true;
			break;
		}
	}

	if (!added){
		runtimeLog << "Added image with zindex " << zi << " to " << objects.size() << endl;
		objects[objects.size()] = mo;
	}
}
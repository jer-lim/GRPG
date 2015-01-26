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

void EntityManager::addEntity(Entity* ent){

	ManagedObject* mo = new ManagedObject();
	mo->entity = ent;
	objects[objects.size()] = mo;
}

void EntityManager::addImage(Image* img){
	ManagedObject* mo = new ManagedObject();
	mo->image = img;
	objects[objects.size()] = mo;
}
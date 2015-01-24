#include "entityManager.h"

using namespace std;

EntityManager::EntityManager(){

}

void EntityManager::updateAll(float frameTime){
	for (int i = 0; i < entities.size(); ++i){
		entities[i]->update(frameTime);
	}
}

void EntityManager::renderAll(){
	for (int i = 0; i < entities.size(); ++i){
		entities[i]->draw();
	}
}

void EntityManager::releaseAll(){
	for (int i = 0; i < entities.size(); ++i){
		entities[i]->getTextureManager()->onLostDevice();
	}
}

void EntityManager::resetAll(){
	for (int i = 0; i < entities.size(); ++i){
		entities[i]->getTextureManager()->onResetDevice();
	}
}

void EntityManager::addEntity(Entity* ent){
	entities[entities.size()] = ent;
}
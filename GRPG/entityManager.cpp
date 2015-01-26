#include "entityManager.h"

using namespace std;

EntityManager::EntityManager(){

}

void EntityManager::updateAll(float frameTime){
	for (int i = 0; i < images.size(); ++i){
		images[i]->update(frameTime);
	}

	for (int i = 0; i < entities.size(); ++i){
		entities[i]->update(frameTime);
	}
}

void EntityManager::renderAll(){
	for (int i = 0; i < images.size(); ++i){
		images[i]->draw();
	}

	for (int i = 0; i < entities.size(); ++i){
		entities[i]->draw();
	}
}

void EntityManager::releaseAll(){
	for (int i = 0; i < images.size(); ++i){
		images[i]->getTextureManager()->onLostDevice();
	}

	for (int i = 0; i < entities.size(); ++i){
		entities[i]->getTextureManager()->onLostDevice();
	}
}

void EntityManager::resetAll(){
	for (int i = 0; i < images.size(); ++i){
		images[i]->getTextureManager()->onResetDevice();
	}

	for (int i = 0; i < entities.size(); ++i){
		entities[i]->getTextureManager()->onResetDevice();
	}
}

void EntityManager::addEntity(Entity* ent){
	entities[entities.size()] = ent;
}

void EntityManager::addImage(Image* img){
	images[images.size()] = img;
}
#include "drawManager.h"
#include "Viewport.h"
#include "entity.h"

using namespace std;

DrawManager::DrawManager(){

}

void DrawManager::initialize(Game* gamPtr, Viewport* vp){
	viewport = vp;
	gamePtr = gamPtr;
}

void DrawManager::updateAll(float frameTime){
	//check if mouse over entity still working
	if (gamePtr->getMouseOverEntity() != nullptr)
	{
		if (!gamePtr->getMouseOverEntity()->mouseInside())
		{
			gamePtr->setMouseOverEntity(nullptr);
		}
	}

	for (map<int, ManagedObject*>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it){
		if (it->second->entity != nullptr)
		{
			it->second->entity->update(frameTime, gamePtr);
			if (gamePtr->getMouseOverEntity() == nullptr)
			{//already have an existing mouse over
				if (it->second->entity->mouseInside())
				{
					gamePtr->setMouseOverEntity(it->second->entity);
				}
			}
		}
		else it->second->image->update(frameTime);
	}
}

void DrawManager::renderAll(){
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->entity != nullptr) it->second->entity->draw(viewport);
		else it->second->image->draw(viewport);
	}
}

void DrawManager::releaseAll(){
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->entity != nullptr) it->second->entity->getTextureManager()->onLostDevice();
		else it->second->image->getTextureManager()->onLostDevice();
	}
}

void DrawManager::resetAll(){
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->entity != nullptr)
		{
			it->second->entity->getTextureManager()->onResetDevice();
			it->second->entity->getImage()->spriteData.texture = it->second->entity->getTextureManager()->getTexture();
		}
		else
		{
			it->second->image->getTextureManager()->onResetDevice();
			it->second->image->spriteData.texture = it->second->image->getTextureManager()->getTexture();
		}
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

	// Compare z-index with existing elements, insert into appropriate position
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->zindex > mo->zindex){
			map<int, ManagedObject*>::iterator it2 = objects.end();
			it2--;
			for (it2; distance(it, it2) >= 0; --it2){
				objects[it2->first + 1] = objects[it2->first];
				if (distance(it, it2) == 0) break;
			}
			objects[it->first] = mo;
			added = true;
			break;
		}
	}

	// Insert to back of map if not inserted anywhere else
	if (!added){
		if (objects.size() == 0){
			objects[0] = mo;
		}
		else{
			map<int, ManagedObject*>::iterator lastIt = objects.end();
			lastIt--;
			int key = lastIt->first;
			objects[++key] = mo;
		}
	}
}

void DrawManager::removeObject(Entity* ent){

	// Iterate to element to delete and delete it
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->entity == ent){
			it = objects.erase(it);
		}
	}
}

void DrawManager::removeObject(Image* img){

	// Iterate to element to delete and delete it
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->image == img){
			it = objects.erase(it);
			break;
		}
	}
}
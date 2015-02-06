// Module: Games Programming
// Assignment 2: GRPG
// Student Name: Jeremy Lim
// Student Number: S10122326F

#include "drawManager.h"
#include "Viewport.h"
#include "entity.h"
#include "grpg.h"
#include "UI.h"
#include <queue>

using namespace std;

DrawManager::DrawManager(){

}

void DrawManager::initialize(Game* gamPtr, Viewport* vp){
	viewport = vp;
	gamePtr = gamPtr;
}

DrawManager::~DrawManager(){
	for (map<int, map<int, ManagedObject*>>::iterator it = objects.begin(); it != objects.end(); ++it){
		int zi = it->first;
		for (map<int, ManagedObject*>::iterator it2 = objects[zi].begin(); it2 != objects[zi].end(); ++it2){
			ManagedObject* mo = it2->second;
			
			if (it2->second->entity)
			{
				SAFE_DELETE(it2->second->entity);
			}
			else if (it2->second->image)
			{
				SAFE_DELETE(it2->second->image);
			}
			delete mo;
			it2->second = nullptr;
		}
	}
	objects.clear();
}

void DrawManager::updateAll(float frameTime){
	try
	{
		if (gamePtr->getMouseOverEntity() != nullptr)
		{	//check if mouse is still over the entity we identified
			if (!gamePtr->getMouseOverEntity()->mouseInside(viewport))
			{
				gamePtr->setMouseOverEntity(nullptr);
			}
		}

		for (map<int, map<int, ManagedObject*>>::reverse_iterator it = objects.rbegin(); it != objects.rend(); ++it){
			int zi = it->first;
			for (map<int, ManagedObject*>::reverse_iterator it2 = objects[zi].rbegin(); it2 != objects[zi].rend(); ++it2){
				if (it2->second->entity != nullptr)
				{
					it2->second->entity->update(frameTime, gamePtr);
					if (gamePtr->getMouseOverEntity() == nullptr)
					{//if don't have an existing mouse over
						if (it2->second->entity->getPerson() != Person::thePlayer)
						{//I don't want to mouse over player (for obvious reasons) nor UI to handle entities acting as my inventory
							if (it2->second->entity->getType() == "UI")
							{//check inventory items							
								if (((UI*)it2->second->entity)->getActiveTab() == uiNS::INVENTORY)
								{
									map<int, Entity*>* slotList = ((Grpg*)gamePtr)->getPlayer()->getInventory()->getSlotList();
									for (std::map<int, Entity*>::iterator it3 = slotList->begin(); it3 != slotList->end(); ++it3)
									{
										if (it3->second->mouseInside(viewport))
										{
											gamePtr->setMouseOverEntity(it3->second);
											break;
										}
									}
								}
								//Check shop items
								vector<Entity*> shopItems = ((UI*)it2->second->entity)->getShopItems();
								for (vector<Entity*>::iterator it3 = shopItems.begin(); it3 != shopItems.end(); ++it3)
								{
									Entity* theItem = *it3;
									if (theItem->mouseInside(viewport))
									{
										gamePtr->setMouseOverEntity(theItem);
										break;
									}
								}
							}
							else if (it2->second->entity->mouseInside(viewport))
							{
								gamePtr->setMouseOverEntity(it2->second->entity);
								break;
							}
						}
					}
				}
				else it2->second->image->update(frameTime);
			}
		}
	}
	catch (...)
	{

	}
}

void DrawManager::renderAll(){
	for (map<int, map<int, ManagedObject*>>::iterator it = objects.begin(); it != objects.end(); ++it){
		int zi = it->first;
		for (map<int, ManagedObject*>::iterator it2 = objects[zi].begin(); it2 != objects[zi].end(); ++it2){
			if (it2->second->entity != nullptr) it2->second->entity->draw(viewport);
			else it2->second->image->draw(viewport);
		}
	}
}

void DrawManager::releaseAll(){
	for (map<int, map<int, ManagedObject*>>::iterator it = objects.begin(); it != objects.end(); ++it){
		int zi = it->first;
		for (map<int, ManagedObject*>::iterator it2 = objects[zi].begin(); it2 != objects[zi].end(); ++it2){
			if (it2->second->entity != nullptr) it2->second->entity->getTextureManager()->onLostDevice();
			else it2->second->image->getTextureManager()->onLostDevice();
		}
	}
}

void DrawManager::resetAll(){
	for (map<int, map<int, ManagedObject*>>::iterator it = objects.begin(); it != objects.end(); ++it){
		int zi = it->first;
		for (map<int, ManagedObject*>::iterator it2 = objects[zi].begin(); it2 != objects[zi].end(); ++it2){
			if (it2->second->entity != nullptr)
			{
				it2->second->entity->getTextureManager()->onResetDevice();
				it2->second->entity->getImage()->spriteData.texture = it2->second->entity->getTextureManager()->getTexture();
			}
			else
			{
				it2->second->image->getTextureManager()->onResetDevice();
				it2->second->image->spriteData.texture = it2->second->image->getTextureManager()->getTexture();
			}
		}
	}
}

void DrawManager::addObject(Entity* ent, int zi){
	ManagedObject* mo = new ManagedObject();
	//runtimeLog << "Created ManagedObject" << endl;
	//runtimeLog << "New memory allocation at 0x" << mo << endl; // NEWLOGGING
	mo->entity = ent;
	
	addManagedObject(zi, mo);
}

void DrawManager::addObject(Image* img, int zi){
	ManagedObject* mo = new ManagedObject();
	//runtimeLog << "Created ManagedObject2" << endl;
	//runtimeLog << "New memory allocation at 0x" << mo << endl; // NEWLOGGING
	mo->image = img;

	addManagedObject(zi, mo);
}

void DrawManager::addManagedObject(int zi, ManagedObject* mo){
	bool added = false;

	// Compare z-index with existing elements, insert into appropriate position
	if (objects[zi].size() == 0){
		objects[zi][0] = mo;
		added = true;
	}
	else {
		map<int, ManagedObject*>::iterator it = objects[zi].end();
		it--;
		int key = it->first + 1;
		objects[zi][key] = mo;

		added = true;
	}

	if (!added) throw new exception;
}

void DrawManager::removeObject(Entity* ent){

	queue<pair<int, int>> toErase;

	// Iterate to element to delete and delete it
	for (map<int, map<int, ManagedObject*>>::iterator it = objects.begin(); it != objects.end(); ++it){
		int zi = it->first;
		for (map<int, ManagedObject*>::iterator it2 = objects[zi].begin(); it2 != objects[zi].end(); ++it2){
			if (it2->second->entity == ent){
				toErase.push(pair<int, int>(zi, it2->first));
			}
		}
	}

	while (!toErase.empty()){
		pair<int, int> p = toErase.front();
		ManagedObject* mo = objects[p.first][p.second];
		delete mo;
		objects[p.first].erase(p.second);
		toErase.pop();
	}
}

void DrawManager::removeObject(Image* img){

	queue<pair<int, int>> toErase;

	// Iterate to element to delete and delete it
	for (map<int, map<int, ManagedObject*>>::iterator it = objects.begin(); it != objects.end(); ++it){
		int zi = it->first;
		for (map<int, ManagedObject*>::iterator it2 = objects[zi].begin(); it2 != objects[zi].end(); ++it2){
			if (it2->second->image == img){
				toErase.push(pair<int, int>(zi, it2->first));
			}
		}
	}

	while (!toErase.empty()){
		pair<int, int> p = toErase.front();
		ManagedObject* mo = objects[p.first][p.second];
		delete mo;
		objects[p.first].erase(p.second);
		toErase.pop();
	}
}
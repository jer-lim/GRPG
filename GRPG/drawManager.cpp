#include "drawManager.h"

using namespace std;

DrawManager::DrawManager(){

}

void DrawManager::updateAll(float frameTime){
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->entity != nullptr) it->second->entity->update(frameTime);
		else it->second->image->update(frameTime);
	}
}

void DrawManager::renderAll(){
	for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->entity != nullptr) it->second->entity->draw();
		else it->second->image->draw();
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
		if (it->second->entity != nullptr) it->second->entity->getTextureManager()->onResetDevice();
		else it->second->image->getTextureManager()->onResetDevice();
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


	/*for (map<int, ManagedObject*>::iterator it = objects.begin(); it != objects.end(); ++it){
		if (it->second->zindex > mo->zindex){
			for (map<int, ManagedObject*>::reverse_iterator it2 = objects.rend()++; it2 > distance(it, objects.begin()); --j){
				objects[j + 1] = objects[j];
			}
			objects[i] = mo;
			added = true;
			break;
		}
	}*/

	if (!added){
		objects[objects.size()] = mo;
	}
}

void DrawManager::removeObject(Entity* ent){
	for (int i = 0; i < objects.size(); ++i){
		if (objects[i]->entity == ent){
			objects.erase(i);
		}
	}
}
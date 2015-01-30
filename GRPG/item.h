#ifndef _ITEM_H                 // Prevent multiple definitions if this 
#define _ITEM_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "game.h"
#include "textureManager.h"

namespace itemNS
{
	const string itemDataLocation = "assets/items/items.gdef";
	const string itemSpriteLocation = "assets/items/sprites/";
	const float spriteWidth = 32;
	const float spriteHeight = 32;
	const float spriteColForMisc = 1;
	const float spriteColForFood = 4;
	const float spriteColForOthers = 7;
}

class Item
{
private:
	//Attributes of a Item
	string name;
	string description;
	string itemImgFileName;
	float spriteWidth, spriteHeight, spriteColumns;
	int maxStackCount;//maximum times item can stack at a slot
	int cost;
	int instanceCount = 0;
	TextureManager* textureManager = nullptr;
public:
	void initializeTexture(Game* gamePtr)
	{
		if (instanceCount <= 0)
		{
			textureManager = new TextureManager();
			if (!textureManager->initialize(gamePtr->getGraphics(), (itemNS::itemSpriteLocation+itemImgFileName).c_str()))
			{
				throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize item texture " + itemImgFileName));
			}
		}
		instanceCount++;
	}
	// constructor
	Item(){}//If you don't have a .cpp, don't put ';', put '{}'
	Item(string n, string d, string i, int m, int c, float sprCol){
		name = n;
		description = d;
		itemImgFileName = i;
		maxStackCount = m;
		cost = c;
		spriteWidth = itemNS::spriteWidth;
		spriteHeight = itemNS::spriteHeight;
		spriteColumns = sprCol;
	}
	void destroy(){ SAFE_DELETE(textureManager); }
	~Item(){ destroy(); }

	string getName(){ return name; }
	void setName(string s){ name = s; }
	string getDescription(){ return description; }
	void setDescription(string s){ description = s; }
	string getItemImgFileName(){ return itemImgFileName; }
	void setItemImgFileName(string s){ itemImgFileName = s; }
	int getMaxStackCount(){ return maxStackCount; }
	void setMaxStackCount(int i){ maxStackCount = i; }
	int getCost() { return cost; }
	void setCost(int i) {cost = i;}
	TextureManager* getTextureManager() { return textureManager; }
	float getSpriteWidth() { return spriteWidth; }
	float getSpriteHeight() { return spriteHeight; }
	float getSpriteColumns() { return spriteColumns; }
	int getInstanceCount() { return instanceCount; }
	void setInstanceCount(int i) { 
		instanceCount = i;
		if (instanceCount <= 0)
		{
			SAFE_DELETE(textureManager);
		}
	}
	void Release() {
		setInstanceCount(instanceCount - 1);
	}
};
#endif
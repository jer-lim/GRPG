//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _ITEM_H                 // Prevent multiple definitions if this 
#define _ITEM_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "game.h"
#include "textureManager.h"
#include <sstream>

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
	int id;
	string name;
	string description;
	string itemImgFileName;
	float spriteWidth, spriteHeight, spriteColumns;
	int maxStackCount;//maximum times item can stack at a slot
	int cost;
	int instanceCount = 0;//Keep track for texturemanager
	TextureManager* textureManager = nullptr;
public:
	TextureManager* initializeTexture(Game* gamePtr)
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
		return textureManager;
	}
	// constructor
	Item(){}//If you don't have a .cpp, don't put ';', put '{}'
	Item(int identity,string n, string d, string i, int m, int c, float sprCol){
		id = identity;
		name = n;
		description = d;
		itemImgFileName = i;
		maxStackCount = m;
		cost = c;
		spriteWidth = itemNS::spriteWidth;
		spriteHeight = itemNS::spriteHeight;
		spriteColumns = sprCol;
	}
	virtual void destroy(){ SAFE_DELETE(textureManager); }
	virtual ~Item(){ destroy(); }

	int getID(){ return id; }
	void setID(int i){ id = i; }
	string getName(){ return name; }
	void setName(string s){ name = s; }
	string getDescription(){ return description; }
	void setDescription(string s){ description = s; }
	string getItemImgFileName(){ return itemImgFileName; }
	void setItemImgFileName(string s){ itemImgFileName = s; }
	int getMaxStackCount(){ return maxStackCount; }
	void setMaxStackCount(int i){ maxStackCount = i; }
	//virtual int getCost() { return cost; } //Singled out and specfied below
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

	virtual int getCost() { return cost; }
	virtual int getSellingCost() { return floor(cost / 10); }

	virtual string getType(){ return "ITEM"; }
};
#endif
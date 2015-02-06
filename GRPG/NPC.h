//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161

#ifndef _NPC_H                 // Prevent multiple definitions if this 
#define _NPC_H				// file is included in more than one place

#include "constants.h"
#include "Person.h"
#include "graphics.h"
#include "entity.h"
#include <vector>
#include <string>
#include <queue>

class PersonLoader;
class DrawManager;

class NPC : public Person
{
private:
	string name;
	string description;
	int maxHealth;
	string talkText;
	//Which tile to teleport to. 0 means none
	char teleportTIleID;
	vector<InventoryItem*>* shopItemsList;

	bool isEnemy;

public:

	NPC() : Person() {}
	NPC(string i, float mov, float atk, float h, float w, int cols, int colHeight, int colWidth, string nama, string desc, int maxhp, bool enemy, vector<InventoryItem*>* shopItems)
		: Person(i, mov, atk, h, w, cols, colHeight, colWidth)
	{
		name = nama;
		description = desc;
		maxHealth = maxhp;
		isEnemy = enemy;
		talkText = "";
		teleportTIleID = '0';
		shopItemsList = shopItems;
	}
	~NPC(){
		if (shopItemsList != nullptr)
		{
			for (int i = 0, l = shopItemsList->size(); i < l; ++i) {
				InventoryItem* ii = shopItemsList->at(i);
				SAFE_DELETE(ii);
			}
			shopItemsList->clear();
			SAFE_DELETE(shopItemsList);
		}
	}

	static Entity* spawn(Game* gamePtr, int npcId, VECTOR2 coords, Entity* victim = nullptr);

	string getname() { return name; }
	string getdescription() { return description; }
	int getmaxhealth() { return maxHealth; }

	bool getIsEnemy() { return isEnemy; }
	string getTalkText() { return talkText; }
	void setTalkText(string txt);
	// Get which tile to teleport to. 0 means none
	char getTeleportID() { return teleportTIleID; }
	void setTeleportID(char t) { teleportTIleID = t; }
	virtual string getType() { return "NPC"; }
	vector<InventoryItem*>* getShopItemsList(){ return shopItemsList; }
	vector<InventoryItem*> getShopItemsListCopy(){
		vector<InventoryItem*> shopList;
		if (shopItemsList != nullptr)
		{
			for (int i = 0, l = shopItemsList->size(); i < l; ++i)
			{
				shopList.push_back(shopItemsList->at(i)->clone());
			}
		}
		return shopList;
	}
};

#endif
#ifndef _NPC_H                 // Prevent multiple definitions if this 
#define _NPC_H				// file is included in more than one place

#include "constants.h"
#include "Person.h"
#include "graphics.h"
#include "entity.h"
#include <vector>
#include <string>

class PersonLoader;
class DrawManager;

class NPC : public Person
{
private:
	string name;
	string description;
	int maxHealth;
	string talkText;
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
	void setTalkText(string txt) { talkText = txt; }
	virtual string getType() { return "NPC"; }
	vector<InventoryItem*>* getShopItemsList(){ return shopItemsList; }
	vector<InventoryItem*> getShopItemsListCopy(){
		vector<InventoryItem*> shopList;
		for (int i = 0, l = shopItemsList->size(); i < l; ++i)
		{
			shopList.push_back(shopItemsList->at(i)->clone());
		}
		return shopList;
	}
};

#endif
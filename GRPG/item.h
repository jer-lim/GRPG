#ifndef _ITEM_H                 // Prevent multiple definitions if this 
#define _ITEM_H                 // file is included in more than one place

#include "constants.h"
#include <string>

namespace itemNS
{
	const string itemDataLocation = "assets/items/items.gdef";
}

class Item
{
private:
	//Attributes of a Item
	string name;
	string description;
	string itemImgFileName;
	int maxStackCount;//maximum times item can stack at a slot
	int cost;
public:
	// constructor
	Item(){}//If you don't have a .cpp, don't put ';', put '{}'
	Item(string n, string d, string i, int m, int c){
		name = n;
		description = d;
		itemImgFileName = i;
		maxStackCount = m;
		cost = c;
	}
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
};
#endif
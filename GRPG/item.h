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
	int useType;//type of item
	long maxStackCount;//maximum times item can stack at a slot
	//long experience;//amt of experience
public:
	// constructor
	//int getSkillLevel();


	// inherited member functions
	//void gainXP(long xp);
};
#endif
#ifndef _ITEMLOADER_H                 // Prevent multiple definitions if this 
#define _ITEMLOADER_H                 // file is included in more than one place

#include "constants.h"
//#include "item.h"
#include "food.h"
#include "Smithing_Material.h"
//#include "Equipment.h"
#include "Weapon.h"
#include "Armor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "string_functions.h"

class ItemLoader
{
private:
	int index_misc_items_start;
	int index_food_start;
	int index_smithing_materials_start;
	int index_weapons_start;
	int index_armor_start;
	int index_shields_start;
	const std::string misc_itemDataLocation = "assets/items/misc_item.gdef";
	const std::string foodDataLocation = "assets/items/food.gdef";
	const std::string smithing_materialsDataLocation = "assets/items/smithing_materials.gdef";
	const std::string weaponsDataLocation = "assets/items/weapons.gdef";
	const std::string armorDataLocation = "assets/items/armor.gdef";
	const std::string shieldsDataLocation = "assets/items/shields.gdef";
	unordered_map<int, Item*> map_items;
public:
	ItemLoader(){}
	void loadAllItems();
	~ItemLoader(){
		for (unordered_map<int, Item*>::iterator it = map_items.begin(); it != map_items.end(); ++it){
			SAFE_DELETE(it->second);
		}
		map_items.clear();
	}
	Item* getItem(int id){
		return map_items[id];
	}
};
#endif
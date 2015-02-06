//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "itemLoader.h"

void ItemLoader::loadAllItems()
{
	ifstream itemstream;
	//Misc Items
	index_misc_items_start = map_items.size();
	itemstream.open(misc_itemDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount, cost;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> stackcount >> cost;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item* myItem = new Item(id,name, descript, img_filename, stackcount, cost, itemNS::spriteColForMisc);
			if (!map_items.count(id))
				map_items[id] = myItem;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Misc Item with same ID " + id));
		}
		itemstream.close();
	}
	//Food items
	index_food_start = map_items.size() + 1;
	itemstream.open(foodDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount, cost, health_gain, levelReq_for_50;
		float level_deviation;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> stackcount 
				>> health_gain >> levelReq_for_50 >> level_deviation >> cost;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item* myItem = new Food(id, name, descript, img_filename, stackcount, cost, health_gain, levelReq_for_50, level_deviation);
			if (!map_items.count(id))
				map_items[id] = myItem;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Food Item with same ID " + id));
		}
		itemstream.close();
	}
	//Smithing Material items
	index_smithing_materials_start = map_items.size() + 1;
	itemstream.open(smithing_materialsDataLocation);
	if (itemstream.is_open()){
		string name, img_filename = "smithing_materials.png", descript;
		int id, stackcount = 1, cost;
		float strMultiplier, defMultiplier, spdMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> descript >> strMultiplier >> defMultiplier >> spdMultiplier >> cost;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item* myItem = new Smithing_Material(id, name, descript, img_filename, stackcount, cost, strMultiplier, defMultiplier, spdMultiplier);
			if (!map_items.count(id))
				map_items[id] = myItem;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Smithing Item with same ID " + id));
		}
		itemstream.close();
	}

	//Weapon items
	index_weapons_start = map_items.size() + 1;
	itemstream.open(weaponsDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount = 1, cost = 1;
		float strMultiplier, spdMultiplier, costMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> strMultiplier >> spdMultiplier >> costMultiplier;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			//CRIME SCENE: ACQUITTED
			Item* myItem = new Weapon(id, name, descript, img_filename, stackcount, cost, strMultiplier, spdMultiplier, costMultiplier, WeaponNS::wpnEqSlot);
			//END CRIME SCENE
			//Item* myItem = new Equipment("fuck", "fuck", "fuck", 0, 0, 2.0f, 0);
			if (!map_items.count(id))
				map_items[id] = myItem;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Weapon Item with same ID " + id));
		}
		itemstream.close();
	}
	//Armor items
	index_armor_start = map_items.size() + 1;
	itemstream.open(armorDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount = 1, cost = 1;
		float DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> DEF_MULTIPLIER >> DMG_REDUCTION >> costMultiplier;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item* myItem = new Armor(id, name, descript, img_filename, stackcount, cost, DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier, 1);//1 -> 001
			if (!map_items.count(id))
				map_items[id] = myItem;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Armor Item with same ID " + id));
		}
		itemstream.close();
	}
	//Shield items
	index_shields_start = map_items.size() + 1;
	itemstream.open(shieldsDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount = 1, cost = 1;
		float DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> DEF_MULTIPLIER >> DMG_REDUCTION >> costMultiplier;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item* myItem = new Armor(id, name, descript, img_filename, stackcount, cost, DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier, 2);//2 -> 010
			if (!map_items.count(id))
				map_items[id] = myItem;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Shield Item with same ID " + id));
		}
		itemstream.close();
	}
}
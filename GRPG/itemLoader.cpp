#include "itemLoader.h"

void ItemLoader::loadAllItems()
{
	ifstream itemstream;
	//Misc Items
	itemstream.open(misc_itemDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount, cost;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> stackcount >> cost;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item myItem = Item(name, descript, img_filename, stackcount, cost);
			map_items[id] = myItem;
		}
		itemstream.close();
	}
	//Food items
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
			Item myItem = Food(name, descript, img_filename, stackcount, cost, health_gain, levelReq_for_50, level_deviation);
			map_items[id] = myItem;
		}
		itemstream.close();
	}
	//Smithing Material items
	itemstream.open(smithing_materialsDataLocation);
	if (itemstream.is_open()){
		string name, img_filename = "smithing_materials.png", descript;
		int id, stackcount = 1, cost;
		float strMultiplier, defMultiplier, spdMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> descript >> strMultiplier >> defMultiplier >> spdMultiplier >> cost;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item myItem = Smithing_Material(name, descript, img_filename, stackcount, cost, strMultiplier, defMultiplier, spdMultiplier);
			map_items[id] = myItem;
		}
		itemstream.close();
	}
	//Weapon items
	itemstream.open(weaponsDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount = 1, cost = 1;
		float strMultiplier, spdMultiplier, costMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> strMultiplier >> spdMultiplier >> costMultiplier;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item myItem = Weapon(name, descript, img_filename, stackcount, cost, strMultiplier, spdMultiplier, costMultiplier);
			map_items[id] = myItem;
		}
		itemstream.close();
	}
	//Armor items
	itemstream.open(armorDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount = 1, cost = 1;
		float DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> DEF_MULTIPLIER >> DMG_REDUCTION >> costMultiplier;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item myItem = Armor(name, descript, img_filename, stackcount, cost, DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier, 1);//1 -> 001
			map_items[id] = myItem;
		}
		itemstream.close();
	}
	//Shield items
	itemstream.open(shieldsDataLocation);
	if (itemstream.is_open()){
		string name, img_filename, descript;
		int id, stackcount = 1, cost = 1;
		float DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier;
		while (!itemstream.eof()){
			itemstream >> id >> name >> img_filename >> descript >> DEF_MULTIPLIER >> DMG_REDUCTION >> costMultiplier;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			Item myItem = Armor(name, descript, img_filename, stackcount, cost, DEF_MULTIPLIER, DMG_REDUCTION, costMultiplier, 2);//2 -> 010
			map_items[id] = myItem;
		}
		itemstream.close();
	}
}
#ifndef _EQUIPMENT_H			         // Prevent multiple definitions if this 
#define _EQUIPMENT_H		             // file is included in more than one place

#include "item.h"
//#include "Smithing_Material.h"

class Equipment : public Item
{
private:
	//Smithing_Material* material;
	float costMultiplier;
	DWORD equipmentSlot = 0x000;
	/*
		000
		ABC, where:
		A: weapon , 0 for does not equip, 1 for equip
		B: offhand weapon, 0 for does not equip, 1 for equip
		C: body, 0 for does not equip, 1 for equip
	*/
public:
	Equipment() :Item(){}
	Equipment(int id,string n, string d, string i, int m, int c, float costM, DWORD eq)
		:Item(id,n, d, i, m, c, itemNS::spriteColForOthers)
	{
		costMultiplier = costM;
		equipmentSlot = eq;
	}

	float getCostMultiplier() { return costMultiplier; }
	void setCostMultiplier(float i){ costMultiplier = i; }
	DWORD getEquipmentSlot() { return equipmentSlot; }
	void setEquipmentSlot(DWORD d){ equipmentSlot = d; }
	bool occupiesHand(){ return equipmentSlot & 4 != 0x000; }//4 -> 100
	bool occupiesOffhand(){ return equipmentSlot & 2 != 0x000; }//2 -> 010
	bool occupiesBody(){ return equipmentSlot & 1 != 0x000; }// 1 -> 001

	//Smithing_Material* getSmithingMaterial() { return material; }
	//void setSmithingMaterial(Smithing_Material* mat){ material = mat; }
};
#endif
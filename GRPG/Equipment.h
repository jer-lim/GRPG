#ifndef _EQUIPMENT_H			         // Prevent multiple definitions if this 
#define _EQUIPMENT_H		             // file is included in more than one place

#include "item.h"

class Equipment : public Item
{
private:
	float costMultiplier;
	DWORD equipmentSlot = 0x000;
	/*
		ABC, where:
		A: weapon , 0 for does not equip, 1 for equip
		B: offhand weapon, 0 for does not equip, 1 for equip
		C: body, 0 for does not equip, 1 for equip
	*/
public:
	Equipment(){}
	Equipment(string n, string d, string i, int m, int c, float costM, DWORD eq)
	{
		Item(n, d, i, m, c);
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
};
#endif
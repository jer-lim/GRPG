#ifndef _SMITHING_MATERIAL_H			         // Prevent multiple definitions if this 
#define _SMITHING_MATERIAL_H		             // file is included in more than one place

#include "item.h"

namespace Smithing_MaterialNS{
	const int smithingMaterialID[]{ 7, 8, 9, 10, 11, 12, 13 };//frame by index of id, id is the smithing material
	const int smithingMaterialIDLength = 7;
}

class Smithing_Material : public Item
{
private:
	float strMultiplier;
	float defMultiplier;
	float spdMultiplier;
public:
	Smithing_Material() :Item(){}
	Smithing_Material(int id,string n, string d, string i, int m, int c, float s, float de, float spd)
		:Item(id,n, d, i, m, c, itemNS::spriteColForOthers)
	{
		strMultiplier = s;
		defMultiplier = de;
		spdMultiplier = spd;
	}

	float getStrMultiplier() { return strMultiplier; }
	void setStrMultiplier(float i){ strMultiplier = i; }
	float getDefMultiplier() { return defMultiplier; }
	void setDefMultiplier(float i){ defMultiplier = i; }
	float getSpdMultiplier() { return spdMultiplier; }
	void setSpdMultiplier(float i){ spdMultiplier = i; }

	int getSmithingMatFrameNo()
	{//forced to dpulicate here because of LNK2019 and lack of time to fix it
		for (int i = 0; i < Smithing_MaterialNS::smithingMaterialIDLength; ++i){//InventoryEquipmentNS::smithingMaterialIDLength
			if (getID() == Smithing_MaterialNS::smithingMaterialID[i])//InventoryEquipmentNS::smithingMaterialID[i])
				return i;
		}
	}

	virtual string getType(){ return "SMITHING_MATERIAL"; }
};
#endif
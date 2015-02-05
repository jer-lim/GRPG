#ifndef _SMITHING_MATERIAL_H			         // Prevent multiple definitions if this 
#define _SMITHING_MATERIAL_H		             // file is included in more than one place

#include "item.h"

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
};
#endif
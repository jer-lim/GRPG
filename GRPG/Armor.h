//MATTSCAN: LIKELIHOOD OF MEMORY LEAK: 0%

#ifndef _ARMOR_H			         // Prevent multiple definitions if this 
#define _ARMOR_H		             // file is included in more than one place

#include "Equipment.h"

//=============================================================================
// Armor also includes shields because they both have the same stats, just different equipment slots
//=============================================================================
class Armor : public Equipment
{
private:
	float defMultiplier;
	float dmgReduction;
public:
	Armor() : Equipment() {}
	Armor(string n, string d, string i, int m, int c, float defM, float dmgM, float costM, DWORD eq)
		: Equipment(n, d, i, m, c, costM, eq)
	{
		defMultiplier = defM;
		dmgReduction = dmgM;
	}

	float getdefMultiplier() { return defMultiplier; }
	void setdefMultiplier(float i){ defMultiplier = i; }
	float getdmgReduction() { return dmgReduction; }
	void setdmgReduction(float d){ dmgReduction = d; }
};
#endif
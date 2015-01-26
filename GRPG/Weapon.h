#ifndef _WEAPON_H			         // Prevent multiple definitions if this 
#define _WEAPON_H		             // file is included in more than one place

#include "Equipment.h"

class Weapon : public Equipment
{
private:
	float strMultiplier;
	float spdMultiplier;
public:
	Weapon(){ Equipment(); }
	Weapon(string n, string d, string i, int m, int c, float strM, float spdM, float costM)
	{
		Equipment(n, d, i, m, c, costM, 6);//6 -> 110
		strMultiplier = strM;
		spdMultiplier = spdM;
	}
	float getstrMultiplier() { return strMultiplier; }
	void setstrMultiplier(float i){ strMultiplier = i; }
	float getspdMultiplier() { return spdMultiplier; }
	void setspdMultiplier(float d){ spdMultiplier = d; }
};
#endif
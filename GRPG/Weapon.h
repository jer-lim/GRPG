//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _WEAPON_H			         // Prevent multiple definitions if this 
#define _WEAPON_H		             // file is included in more than one place

#include "Equipment.h"

namespace WeaponNS
{
	const DWORD wpnEqSlot = 6;
}

class Weapon : public Equipment
{
private:
	float strMultiplier;
	float spdMultiplier;
public:
	Weapon():Equipment(){ }

	Weapon(int id,string n, string d, string i, int m, int c, float strM, float spdM, float costM, DWORD eq)
		:Equipment(id,n, d, i, m, c, costM, eq)//6 -> 110, cannot be defined in constructor
	{
		strMultiplier = strM;
		spdMultiplier = spdM;
	}
	float getstrMultiplier() { return strMultiplier; }
	void setstrMultiplier(float i){ strMultiplier = i; }
	float getspdMultiplier() { return spdMultiplier; }
	void setspdMultiplier(float d){ spdMultiplier = d; }

	virtual string getType(){ return "WEAPON"; }
};
#endif
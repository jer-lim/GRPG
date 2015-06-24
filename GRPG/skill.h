//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

//MATTSCAN: LIKELIHOOD OF MEMORY LEAK: NO WAY
/*
	MEMORY LEAK POSSIBILITIES:
		ALL SKILL POINTERS: DESTRUCTOR CALLED BY ~GRPG
*/

#ifndef _SKILL_H                 // Prevent multiple definitions if this 
#define _SKILL_H                 // file is included in more than one place

#include "constants.h"
#include <string>

namespace skillNS
{
	const int ID_SKILL_ATTACK = 0;
	const int ID_SKILL_STRENGTH = 1;
	const int ID_SKILL_DEFENSE = 2;
	const int ID_SKILL_TOUGHNESS = 3;
	const int ID_SKILL_FISHING = 4;
	const int ID_SKILL_COOKING = 5;
	const int ID_SKILL_MINING = 6;
	const int ID_SKILL_THIEVING = 7;
	const int ID_SKILL_RIFTSEALING = 8;
	const int skillInterval = 100;// The intervals between skills in terms of EXP
	const int maxLevel = 99;//maximum level
}

class Skill
{
private:
	//Attributes of a Skill
	std::string name;//name of skill
	std::string description;//description of skill
	int exp_gain;//skill gain per action done
public:
	//Skill names and what they do. These skills are defined in the cpp file
	static Skill* ATTACK;
	static Skill* STRENGTH;
	static Skill* DEFENSE;
	static Skill* TOUGHNESS;
	static Skill* FISHING;
	static Skill* COOKING;
	static Skill* MINING;
	static Skill* THIEVING;
	static Skill* RIFT_SEALING;
	static void setupAllSkills();
    // constructor
    Skill();
	Skill(std::string,std::string,int);
	~Skill();

	//Get functions

	std::string getName() const
	{
		return name;
	}

	std::string getDescription() const
	{
		return description;
	}

	int getExpGain() const
	{
		return exp_gain;
	}

	//Currently every level requires 80 base XP, plus 20 more XP per level after 2.
	//(So level 1->2 = 80 XP, 2->3 = 100XP, 3->4 = 120 XP, so on...
	static int calculateXPRequired(float level)
	{
		float points = 0;
		for (float lvl = 1; lvl < level; lvl++)
		{
			points += 60 + lvl * 20;
		}
		return points;
	}

	static void deleteAllSkills()
	{
		SAFE_DELETE(ATTACK);
		SAFE_DELETE(STRENGTH);
		SAFE_DELETE(DEFENSE);
		SAFE_DELETE(TOUGHNESS);
		SAFE_DELETE(FISHING);
		SAFE_DELETE(COOKING);
		SAFE_DELETE(MINING);
		SAFE_DELETE(THIEVING);
	}
};
#endif


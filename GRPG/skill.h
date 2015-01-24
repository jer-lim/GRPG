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
	const int skillInterval = 100;// The intervals between skills in terms of EXP
	const int maxLevel = 99;//maximum level
	const std::string skillDataLocation = "assets/skills/skills.gdef";
}

class Skill
{
private:
	//Attributes of a Skill
	std::string name;//name of skill
	std::string description;//description of skill
	int exp_gain;//skill gain per action done
public:
    // constructor
    Skill();
	Skill(std::string,std::string,int);
};
#endif


#ifndef _SKILL_H                 // Prevent multiple definitions if this 
#define _SKILL_H                 // file is included in more than one place

#include "constants.h"
#include <string>

namespace skillNS
{
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
	//Skill names and what they do
	static const Skill ATTACK;
	static const Skill STRENGTH;
	static const Skill DEFENSE;
	static const Skill TOUGHNESS;
	static const Skill FISHING;
	static const Skill COOKING;
	static const Skill MINING;

	static void initSkills();

    // constructor
    Skill();
	Skill(std::string,std::string,int);

	//Uses runescape's xp formula currently
	static int calculateXPRequired(float level)
	{
		float points = 0;
		for (float lvl = 1; lvl <= level; lvl++)
		{
			points += floor(lvl + 300 * pow(2, lvl / 7));
		}
		return floor(points / 4);
	}
};
#endif


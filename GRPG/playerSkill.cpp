#include "playerSkill.h"
#include <cmath>
#include <sstream>

//=============================================================================
// default constructor
//=============================================================================
PlayerSkill::PlayerSkill() {}

//=============================================================================
// Parameterized constructor.
// p: The player that has this skill
// s: The skill that you are referring to
//=============================================================================
PlayerSkill::PlayerSkill(Player* p, Skill s)
{
	thePlayer = p;
	skill = s;
	experience = 0;
}

//Makes use of Runescape's experience formula to calculate the level
int PlayerSkill::getSkillLevel()
{
	float points = 0;
	for (float lvl = 1; lvl <= 99; lvl++)
	{
		points += floor(lvl + 300 * pow(2, lvl / 7.));
		if (points / 4 > experience)
		{
			return lvl - 1;
		}
	}
	return 99;
}

void PlayerSkill::gainXP(long XP)
{
	experience += XP;
}
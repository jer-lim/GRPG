//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

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
PlayerSkill::PlayerSkill(Player* p, Skill* s)
{
	thePlayer = p;
	skill = s;
	experience = 0;
}

//Destructor
void PlayerSkill::destroy()
{
	thePlayer = nullptr;
	skill = nullptr;//all skills deleted on their own
}
PlayerSkill::~PlayerSkill()
{
	destroy();
}

//Makes use of Runescape's experience formula to calculate the level
int PlayerSkill::getSkillLevel()
{
	float points = 0;
	for (float lvl = 2; lvl <= 99; lvl++)
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
	if (XP != -1)
		experience += XP;
	else
		experience += skill->getExpGain();
}
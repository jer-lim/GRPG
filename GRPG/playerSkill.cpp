#include "playerSkill.h"

//=============================================================================
// default constructor
//=============================================================================
PlayerSkill::PlayerSkill()
{
	experience = 0;
}

int PlayerSkill::getSkillLevel()
{
	return 1;
}

void PlayerSkill::gainXP(long XP)
{
	experience += XP;
}
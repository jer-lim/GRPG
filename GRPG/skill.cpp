#include "skill.h"

//=============================================================================
// default constructor
//=============================================================================
Skill::Skill()
{
	experience = 0;
}

int Skill::getSkillLevel()
{
	return 1;
}

void Skill::gainXP(long XP)
{
	experience += XP;
}
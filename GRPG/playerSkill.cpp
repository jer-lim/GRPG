//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "playerSkill.h"
#include <cmath>
#include <sstream>
#include "player.h"
#include "NoiseManager.h"
#include "UI.h"

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
	level = 1;
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
	return level;
}

void PlayerSkill::hasLevelledUp()
{
	for (int i = level; i <= 99; i++)
	{
		if (skill->calculateXPRequired(i) > experience)
		{
			level = i - 1;
			return;
		}
	}
	level = 99;
}

void PlayerSkill::gainXP(long XP, bool skip)
{
	int currentSkillLevel = getSkillLevel();
	if (XP != -1)
		experience += XP;
	else
		experience += skill->getExpGain();

	hasLevelledUp();

	if (!skip)
	{
		if (currentSkillLevel < getSkillLevel())
		{
			int difference = getSkillLevel() - currentSkillLevel;
			stringstream ss;
			ss << "Congratulations! You have just achieved " << difference << " more level";
			if (difference > 1)
			{
				ss << "s";
			}
			ss << " in " << skill->getName();
			thePlayer->getUI()->addChatText(ss.str());
			thePlayer->getUI()->newSkillLevelUP(skill->getName());
			SoundManager::playSound(soundManagerNS::levelUpSoundFileName);
		}
	}
}
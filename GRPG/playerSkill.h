#ifndef _PLAYER_SKILL_H                 // Prevent multiple definitions if this 
#define _PLAYER_SKILL_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include <vector>
#include "skill.h"

//Why do we need this? Well the compiler needs to know about player so that it can properly define
//the player variable. Why can't we just include "player.h"? Well player.h includes playerSkill.h
//as well cause it needs it to define the player's skill. Circular reference! Fix this by
//telling the compile the Player class exists ahead of time and only defining pointers to it
//For more information, check out https://stackoverflow.com/questions/625799/resolve-circular-dependencies-in-c
class Player;

class PlayerSkill
{
private:
	//Attributes of a Skill
	//std::string name;//name of skill
	//std::string description;//description of skill
	//int exp_gain;//skill gain per action done

	long experience;//amt of experience
	//The player that has this skill and it's corresponding XP
	Player* thePlayer;
	//The skill that this refers to
	Skill skill;
public:
	// default constructor
	PlayerSkill();
	// constructor
	PlayerSkill(Player* referencedPlayer, Skill s);

	//Get functions

	// Get the player that has this skill
	Player* getPlayer() { return thePlayer; }

	// Get the skill that this referes to
	Skill getSkill() { return skill; }

	//Makes use of Runescape's experience formula to calculate the level
	int getSkillLevel();

	// inherited member functions
	void gainXP(long xp);
};
#endif
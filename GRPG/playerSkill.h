//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _PLAYER_SKILL_H                 // Prevent multiple definitions if this 
#define _PLAYER_SKILL_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include <vector>
#include "skill.h"

//Why do we need this? Well the compiler needs to know about player so that it can properly define
//the player variable. Why can't we just include "player.h"? Well player.h includes playerSkill.h
//as well cause it needs it to define the player's skill. Circular reference! Fix this by
//telling the compiler the Player class exists ahead of time and only defining pointers to it
//For more information, check out https://stackoverflow.com/questions/625799/resolve-circular-dependencies-in-c
class Player;
class UI;

class PlayerSkill
{
private:
	//Attributes of a Skill
	//std::string name;//name of skill
	//std::string description;//description of skill
	//int exp_gain;//skill gain per action done

	int experience;//amt of experience
	//The player that has this skill and it's corresponding XP
	Player* thePlayer;
	//The skill that this refers to
	Skill* skill;
	//The level
	int level;
public:
	// default constructor
	PlayerSkill();
	// constructor
	PlayerSkill(Player* referencedPlayer, Skill* s);

	void destroy();
	~PlayerSkill();

	//Get functions

	// Get the player that has this skill
	Player* getPlayer() { return thePlayer; }

	// Get the skill that this referes to
	const Skill* getSkill() { return skill; }

	// Get the total number of XP the player has gained in this skill
	int getXP() { return experience; }

	// Set the total number of XP the player has gained in this skill
	// Used for loading save files.
	void setXP(int xp) { experience = xp; hasLevelledUp(); }

	//Returns the current skill level
	int getSkillLevel();

	// inherited member functions
	// Adds the specified amount of xp to the skill
	// IF XP is -1, adds the default amount of xp. -1 by default.
	// Skip::Whether to display the level up message if a level up occurs. False by default.
	void gainXP(long xp = -1, bool skip = false);

	//Attempts to check if a new level up has occured, updates level variable.
	void hasLevelledUp();
};
#endif
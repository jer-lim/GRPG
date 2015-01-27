#ifndef _PERSON_H                 // Prevent multiple definitions if this 
#define _PERSON_H				// file is included in more than one place

#include "constants.h"
#include <string>

namespace PersonNS{
	const std::string spriteDirectory = "assets/npcs/sprites/";

	//IDs
	const int ID_NPC_DOCTOR = 0;
	const int ID_NPC_GROCER = 1;
	const int ID_NPC_BLACKSMITH = 2;

	const int ID_NPC_SKELETON = 3;
	const int ID_NPC_SKELETON_KING = 4;
	const int ID_NPC_GOBLIN = 5;
	const int ID_NPC_GOBLIN_CHIEF = 6;
	const int ID_NPC_UNDEAD = 7;
	const int ID_NPC_UNDEAD_CMDR = 8;
	const int ID_NPC_OGRE = 9;
	const int ID_NPC_OGRE_BOSS = 10;
	const int ID_NPC_GOLEM = 11;
	const int ID_NPC_GOLEM_GUARDIAN = 12;
	const int ID_NPC_DRAGON = 13;
	const int ID_NPC_AIDIL = 14;
}

class Person
{
private:
	string imgFileName;
	float movementSpeed;
	float attackCooldown;

	float height;
	float width;
	int numOfCols;

public:
	//Static characters used to refer to them (Similar to skills)
	static Person* thePlayer;

	Person(){}
	Person(string i, float mov, float atk, float h, float w, int cols)
	{
		imgFileName = i;
		movementSpeed = mov;
		attackCooldown = atk;
		height = h;
		width = w;
		numOfCols = cols;
	}

	string getImgFileName() { return imgFileName; }
	float getMovementSpeed() { return movementSpeed; }
	float getAttackCooldown() { return attackCooldown; }
	float getHeight() { return height; }
	float getWidth() { return width; }
	int getNumOfCols() { return numOfCols; }
};

#endif
#ifndef _CHARACTER_H                 // Prevent multiple definitions if this 
#define _CHARACTER_H				// file is included in more than one place

#include "constants.h"

class Character
{
private:
	const char* imgFileName;
	float movementSpeed;
	float attackCooldown;

public:
	//Static characters used to refer to them (Similar to skills
	static Character* thePlayer;

	Character(){}
	Character(const char* i, float mov, float atk)
	{
		imgFileName = i;
		movementSpeed = mov;
		attackCooldown = atk;
	}

	const char* getImgFileName() { return imgFileName; }
	const float getMovementSpeed() { return movementSpeed; }
	const float getAttackCooldown() { return attackCooldown; }
};

#endif
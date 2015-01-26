#ifndef _CHARACTER_H                 // Prevent multiple definitions if this 
#define _CHARACTER_H				// file is included in more than one place

#include "constants.h"

class Character
{
private:
	char* imgFileName;
	float movementSpeed;
	float attackCooldown;

public:
	Character(){}
	Character(char* i, float mov, float atk)
	{
		imgFileName = i;
		movementSpeed = mov;
		attackCooldown = atk;
	}
};

#endif
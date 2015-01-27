#ifndef _PERSON_H                 // Prevent multiple definitions if this 
#define _PERSON_H				// file is included in more than one place

#include "constants.h"
#include <string>

class Person
{
private:
	string imgFileName;
	float movementSpeed;
	float attackCooldown;

public:
	//Static characters used to refer to them (Similar to skills)
	static Person* thePlayer;

	Person(){}
	Person(string i, float mov, float atk)
	{
		imgFileName = i;
		movementSpeed = mov;
		attackCooldown = atk;
	}

	string getImgFileName() { return imgFileName; }
	float getMovementSpeed() { return movementSpeed; }
	float getAttackCooldown() { return attackCooldown; }
};

#endif
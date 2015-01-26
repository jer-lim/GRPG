#ifndef _CHARACTER_H                 // Prevent multiple definitions if this 
#define _CHARACTER_H				// file is included in more than one place

#include "constants.h"
#include <string>

class Character
{
private:
	string imgFileName,
		name,
		description;
	int maxHealth;
	float movementSpeed,
		attackCooldown,
		base_dmgReduction;
	DWORD player_actions = 0x000;
	/*
		0000
		ABCD, where:
		A: Attackable
		??
	*/
public:
	Character(){}
	Character(string i, string n, string d, int hp, float mov, float atk, float red){
		imgFileName = i;
		name = n;
		description = d;
		maxHealth = hp;
		movementSpeed = mov;
		attackCooldown = atk;
		base_dmgReduction = red;
	}
	bool attack(Character otherChar){
	}
	bool takeDamage(Character otherChar){
	}
	void destroy(){

	}
};

#endif
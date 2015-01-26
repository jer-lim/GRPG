#ifndef _ENEMY_H			         // Prevent multiple definitions if this 
#define _ENEMY_H		             // file is included in more than one place

#include "Character.h"

class Enemy : public Character
{
private:
	int damage, attackLv, defenseLv, strengthLv;
	float damageReduction;
public:
	static Enemy* Skeleton;

	Enemy(){ Character(); }

	Enemy(const char* i, float mov, float atk, int d, int atkLv, int defLv, int strLv, float dmgReduction) : Character(i, mov, atk)
	{
		damage = d;
		attackLv = atkLv;
		defenseLv = defLv;
		strengthLv = strLv;
		damageReduction = dmgReduction;
	}

	//Getters and setters

	int getDamage(){ return damage; }
	void setDamage(int i){ damage = i; }
	int getattackLv(){ return attackLv; }
	void setattackLv(int i){ attackLv = i; }
	int getdefenseLv(){ return defenseLv; }
	void setdefenseLv(int i){ defenseLv = i; }
	int getstrengthLv(){ return strengthLv; }
	void setstrengthLv(int i){ strengthLv = i; }
};

#endif
#ifndef _ENEMY_H			         // Prevent multiple definitions if this 
#define _ENEMY_H		             // file is included in more than one place

#include "Character.h"

class Enemy : public Character
{
private:
	int damage, attackLv, defenseLv, strengthLv;
public:
	Enemy(){ Character(); }
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
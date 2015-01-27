#ifndef _ENEMY_H			         // Prevent multiple definitions if this 
#define _ENEMY_H		             // file is included in more than one place

#include "NPC.h"

class Enemy : public NPC
{
private:
	int attackLv, defenseLv, strengthLv;
	float damageReduction;
public:
	//static Enemy* Skeleton;

	Enemy() : NPC() { }

	Enemy(string i, float mov, float atkSpd, float h, float w, float cols, float colHeight, float colWidth, string nama, string desc, int maxhp, int atkLv, int defLv, int strLv, float dmgReduction) 
		: NPC(i, mov, atkSpd, h, w, cols, colHeight, colWidth, nama, desc, maxhp)
	{
		attackLv = atkLv;
		defenseLv = defLv;
		strengthLv = strLv;
		damageReduction = dmgReduction;
	}

	//Getters and setters
	int getattackLv(){ return attackLv; }
	void setattackLv(int i){ attackLv = i; }
	int getdefenseLv(){ return defenseLv; }
	void setdefenseLv(int i){ defenseLv = i; }
	int getstrengthLv(){ return strengthLv; }
	void setstrengthLv(int i){ strengthLv = i; }
};

#endif
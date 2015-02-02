#ifndef _ENEMY_H			         // Prevent multiple definitions if this 
#define _ENEMY_H		             // file is included in more than one place

#include "NPC.h"
#include <vector>

namespace enemyNS
{
	const float aggroRangeNonSqrt = 500 * 500;
}

class Enemy : public NPC
{
private:
	int attackLv, defenseLv, strengthLv;
	int aggro; //The enemy will attack the palyer if the player's combat level is equal to this level or lower
	int wanderRange;
	float damageReduction;
	vector<InventoryItem*>* dropsList;
	
public:
	//static Enemy* Skeleton;

	Enemy() : NPC() { }
	~Enemy(){
		for (int i = 0, l = dropsList->size(); i < l;++i) {
			InventoryItem* ii = dropsList->at(i);
			SAFE_DELETE(ii);
		}
		dropsList->clear();
		SAFE_DELETE(dropsList);
	}

	Enemy(string i, float mov, float atkSpd, float h, float w, float cols, float colHeight, float colWidth, string nama, string desc, int maxhp, int atkLv, int defLv, int strLv, float dmgReduction, int a, int range, vector<InventoryItem*>* drops)
		: NPC(i, mov, atkSpd, h, w, cols, colHeight, colWidth, nama, desc, maxhp, true)
	{
		attackLv = atkLv;
		defenseLv = defLv;
		strengthLv = strLv;
		damageReduction = dmgReduction;
		aggro = a;
		wanderRange = range;
		dropsList = drops;
	}

	//Getters and setters
	int getattackLv(){ return attackLv; }
	//void setattackLv(int i){ attackLv = i; }
	int getdefenseLv(){ return defenseLv; }
	//void setdefenseLv(int i){ defenseLv = i; }
	int getstrengthLv(){ return strengthLv; }
	//void setstrengthLv(int i){ strengthLv = i; }
	int getAggro() { return aggro; }
	int getWanderRange() { return wanderRange; }

	vector<InventoryItem*>* getDropsList() { return dropsList; }

	virtual string getType(){ return "ENEMY"; }
};

#endif
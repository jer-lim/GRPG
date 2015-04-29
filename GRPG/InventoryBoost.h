#ifndef _INVENTORY_BOOST_H                 // Prevent multiple definitions if this 
#define _INVENTORY_BOOST_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"
#include "Boost.h"
#include "UI.h"
#include "player.h"

class InventoryBoost : public InventoryItem
{
private:
	
public:
	// constructor
	InventoryBoost() : InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	InventoryBoost(Item* item, int csc)
		:InventoryItem(item, csc)
	{
		
	}
	virtual string getType() { return "INVENTORYBOOST"; }
	
	virtual InventoryBoost* clone() {
		return new InventoryBoost(*this);
	}

	virtual void performBoost(UI* ui, Player* p)
	{
		int totalSkills = p->getSkills()->size();
		int skillChosen = rand() % totalSkills;
		string skillName = p->getSkills()->at(skillChosen).getSkill()->getName();
		stringstream ss;
		ss << "You gained " << ((Boost*)item)->getXPGain() << " XP in " << skillName;
		ui->addChatText(ss.str());
		p->getSkills()->at(skillChosen).gainXP(((Boost*)item)->getXPGain());
	}
};
#endif
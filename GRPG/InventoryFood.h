#ifndef _INVENTORY_FOOD_H                 // Prevent multiple definitions if this 
#define _INVENTORY_FOOD_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"

namespace InventoryFoodNS
{
	const float foodStateCost[]{ 1, 2, 0, 3};
}

class InventoryFood : public InventoryItem
{
private:
	int foodState;
public:
	// constructor
	InventoryFood():InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	InventoryFood(Item* item, int csc,int state)
		:InventoryItem(item, csc)
	{
		foodState = state;
	}
	int getFoodState(){ return foodState; }
	int setFoodState(int i){ foodState = i; }

	virtual string getType() { return "INVENTORYFOOD"; }

	virtual InventoryFood* clone() {
		return new InventoryFood(*this);
	}

	virtual int getCost(){
		return item->getCost()*InventoryFoodNS::foodStateCost[foodState];
	}

	virtual int getSellingCost(){
		return item->getSellingCost()*InventoryFoodNS::foodStateCost[foodState];
	}
};
#endif
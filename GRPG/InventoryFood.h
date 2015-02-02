#ifndef _INVENTORY_FOOD_H                 // Prevent multiple definitions if this 
#define _INVENTORY_FOOD_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"
#include "Food.h"

namespace InventoryFoodNS
{
	const float foodStateCost[]{ 1, 2, 0, 3};
}

class InventoryFood : public InventoryItem
{
private:
	FOOD_STATE foodState;
public:
	// constructor
	InventoryFood():InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	InventoryFood(Item* item, int csc,FOOD_STATE state)
		:InventoryItem(item, csc)
	{
		foodState = state;
	}
	FOOD_STATE getFoodState(){ return foodState; }
	FOOD_STATE setFoodState(FOOD_STATE i){ foodState = i; }

	virtual string getType() { return "INVENTORYFOOD"; }

	virtual InventoryFood* clone() {
		return new InventoryFood(*this);
	}

	FOOD_STATE cook(int cookingLevel){
		/* generate secret number between 1 and 100: */
		int chance = rand() % 100 + 1;
		int requiredChance = 50 - (cookingLevel - ((Food*)item)->getlevelReq_for_50())*((Food*)item)->getlevel_deviation();
		if (chance > requiredChance)
		{//successfully cooked
			if (chance - requiredChance > FoodNS::DELICIOUS_CHANCE_DIFFERENCE)
				foodState = DELICIOUS;
			else
				foodState = COOKED;
		}
		foodState = BURNT;
		return foodState;
	}

	virtual int getCost(){
		return item->getCost()*InventoryFoodNS::foodStateCost[foodState];
	}

	virtual int getSellingCost(){
		return item->getSellingCost()*InventoryFoodNS::foodStateCost[foodState];
	}
};
#endif
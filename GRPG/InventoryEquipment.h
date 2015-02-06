//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _INVENTORY_EQUIPMENT_H                 // Prevent multiple definitions if this 
#define _INVENTORY_EQUIPMENT_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"
#include "Smithing_Material.h"

namespace InventoryEquipmentNS
{
	const int smithingMaterialID[]{ 7, 8, 9, 10, 11, 12, 13 };//frame by index of id, id is the smithing material
	const int smithingMaterialIDLength = 7;
}

class InventoryEquipment : public InventoryItem
{
private:
	Smithing_Material* smithingMaterial;

public:
	int getFrameNo(){
		for (int i = 0; i < InventoryEquipmentNS::smithingMaterialIDLength; ++i){
			if (smithingMaterial->getID() == InventoryEquipmentNS::smithingMaterialID[i])
				return i;		
		}
	}

	// constructor
	InventoryEquipment() :InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	InventoryEquipment(Item* item, int csc, Smithing_Material* material)
		:InventoryItem(item, csc)
	{
		smithingMaterial = material;
	}
	virtual ~InventoryEquipment()
	{
		SAFE_RELEASE(smithingMaterial);
	}

	Smithing_Material* getSmithingMaterial() { return smithingMaterial; }
	void setSmithingMaterial(Smithing_Material* mat){ smithingMaterial = mat; }

	virtual string getType() { return "INVENTORYEQUIPMENT"; }

	virtual InventoryEquipment* clone() {
		return new InventoryEquipment(*this);
	}

	virtual string getName()
	{
		return smithingMaterial->getName()+" "+item->getName() + "(" + currentStackCountString + ")";
	}

	virtual int getCost(){
		return item->getCost()*smithingMaterial->getCost();
	}

	virtual int getSellingCost(){
		return item->getSellingCost()*smithingMaterial->getCost();
	}
};
#endif
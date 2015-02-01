#ifndef _INVENTORY_EQUIPMENT_H                 // Prevent multiple definitions if this 
#define _INVENTORY_EQUIPMENT_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"
#include "Smithing_Material.h"

class InventoryEquipment : public InventoryItem
{
private:
	Smithing_Material* smithingMaterial;
public:
	// constructor
	InventoryEquipment() :InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	InventoryEquipment(Item* item, int csc, Smithing_Material* material)
		:InventoryItem(item, csc)
	{
		smithingMaterial = material;
	}

	Smithing_Material* getSmithingMaterial() { return smithingMaterial; }
	void setSmithingMaterial(Smithing_Material* mat){ smithingMaterial = mat; }
};
#endif
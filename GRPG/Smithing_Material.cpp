#include "Smithing_Material.h"
#include "InventoryItem.h"

int Smithing_Material::getFrameNo()
{
	for (int i = 0; i < InventoryEquipmentNS::smithingMaterialIDLength; ++i){
		if (getID() == InventoryEquipmentNS::smithingMaterialID[i])
			return i;
	}
}
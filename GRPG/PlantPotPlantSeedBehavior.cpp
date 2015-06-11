#include "PlantPotPlantSeedBehavior.h"
#include "PlantPatch.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"

string PlantPotPlantSeedBehavior::displayText(){
	return "Plant seed in pot";
}

void PlantPotPlantSeedBehavior::action()
{
	if (!planted)
	{
		ui->addChatText("You dig a little hole in the plant pot and place the seed inside.");
		ui->addChatText("Now to wait for it to mature.");
		questData->setValue("mysteriousArtifactWaitTime", 2);
		planted = true;
	}
	else
	{
		ui->addChatText("The seed has already been planted.");
	}
}
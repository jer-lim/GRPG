#ifndef _QUESTDATA_H
#define _QUESTDATA_H

#include <string>
#include <map>

using namespace std;

//Class to store all the data within quests so that entity in the game can be automatically updated
//For example, during certain quests when action needs to be enabled
//Also tracks complettion of specific quests so that the information can be retrieved, and then the rest
//of the game can be tracked accordingly.
class QuestData
{
private:
public:
	//Easter quest data
	map<string, int> storedInformation;

	//Sets the required key in the quest data to the new value
	//If no such key exists, then creates the key and sets it.
	//Otherwise, overrides that key to the new value.
	virtual void setValue(string key, int value)
	{
		storedInformation[key] = value;
	}

	virtual int getValue(string key)
	{
		return storedInformation[key];
	}

	//Initalizes storedInformation with some values as defined in the init method in QuestData.h
	//Warning: These values are also used throughout the entire game to track quests. Notable places
	//are when enemy dies, or when an enemy spawns. Basically, whatever effects the quest has
	//at the end will be tracked at that location.
	//Additionally, changes will be made throughout the course of quests by GameEvents. You can track
	//all of these changes at QuestLoader.cpp
	virtual void init()
	{
		//Represents the information about the easter quest on how it is progressing
		//0 = not started, 1 = started, 3 tasks required , 2 = soothng required, 3 = soothing finished
		storedInformation["easterStatus"] = 0;
		storedInformation["easterComplete"] = 0;
		//0 = not yet, 1 = feather required, 2 = nest rebuild required, 3 = done
		storedInformation["easterBirdNestStatus"] = 0;
		//0 = not yet, 1 = egg required, 2 = egg needs to be given, 3 = done
		storedInformation["easterEggStatus"] = 0;
		storedInformation["foodGiven"] = 0;
		//Indicates if a delicious food was given to the easter bird or not
		//If 1, delicious rotten meat was given, otherwise, 0 if only cooked.
		//If delicious was given, then an extra 1000 cooking xp is awarded.
		storedInformation["easterGoodFoodGiven"] = 0;

		//Represents the information about the smoke quest and how it is progressing
		//0 = not started, 1 = started, 3 tasks required, 2 = 
		storedInformation["smokeStatus"] = 0;
		storedInformation["smokeComplete"] = 0;
		//0 = not started, 1 = started quest
		storedInformation["elliotStatus"] = 0;
		//0 = not started, 1 = started quest
		storedInformation["geraldStatus"] = 0;
		//0 = not started, 1 = started quest
		storedInformation["mattStatus"] = 0;

		//Represents the information about the stolen artifact quest and how it is progressing
		//0 = not started, 1 = started, 2 = House Entry opened, 3 = artifact retrieved, 4 = artifact returned
		storedInformation["artifactStealStatus"] = 0;
		//0 = no key, 1 = player has key
		storedInformation["artifactStealKey"] = 0;
		//0 = artifactDisplayClosed, 1 = artifactDisplayOpen
		storedInformation["artifactDisplayStatus"] = 0;
		//Method in which the player used to get the door open
		//0 = unknown; 1 = fishing, 2 = thieving, 3 = fighting, 4 = toughness test
		storedInformation["artifactStealMethod"] = 0;

		//Represents if the player admitted the theft to Alfred or not.
		storedInformation["artifactStealAdmitted"] = 0;

		//Represents the information about the mysterious artifact quest and how it is progressing
		//0 = not started, 1 = started, 2 = progressed - rift opening ready, 3 = rift opened, 4 = entry OK, 5 = rift entered
		//6 = monsters summoned in rift, 7 = rift exited, 8 - quest complete!
		storedInformation["mysteriousArtifactStatus"] = 0;
		//Represents if the player has asked the gardener if he can chose to start on a task other than planting flowers.
		storedInformation["mysteriousArtifactOtherTask"] = 0;
		//Represents the chosen task the player has selected.
		//0 = none yet, 1 = plant flowers, 2 = plant trees, 3 = add fishes.
		storedInformation["mysteriousArtifactGardenerTask"] = 0;
		//Represents if the player needs to wait for anything to grow
		//0 = nope, 1 = the flowers in the patch, 2 = the young tree in the plant pot in inventory, 3 = the tree in patch
		storedInformation["mysteriousArtifactWaitTime"] = 0;
		//Represents if the player asked the shriveled man what the dark realm is called
		storedInformation["darkRealmName"] = 0;
		//Represents if Alfred's reward from the Mysterious Artifact quest has been given.
		storedInformation["mysteriousArtifactAlfredReward"] = 0;

		//Represents the information about the One Minor Task quest and how it is progressing
		//0 = not started, 1 = started, 2 = fisherman illness revealed, 3 = doctor talked and walking to fisherman, 4 = fisherman illness resolved, 5 = Quest Complete!
		storedInformation["minorTaskStatus"] = 0;
		//Represents if the player has asked Captain Point of No Return why he can't bring the net himself.
		storedInformation["minorTaskAskerBringNet"] = 0;
		//Represents if the player has asked Captain Point of No Return why he needs the net.
		storedInformation["minorTaskAskerNeedsNet"] = 0;
		//Represents if the player complained to the captain about the quest after the fisherman asked for a favour as well
		storedInformation["minorTaskComplained"] = 0;

		//Miscallenous data
		//Represents whether the player has asked the doctor about the save crystal or not, and the result
		//0 = not yet, 1 = asked, said believe, 2 = asked, said didn't believe
		storedInformation["saveCrystalDoctorAsked"] = 0;
	}

	virtual map<string, int> getAllValues() { return storedInformation; }

	virtual void loadQuestData(map<string, int> data)
	{
		storedInformation = data;
	}
};

#endif
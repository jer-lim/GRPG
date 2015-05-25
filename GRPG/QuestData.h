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

		//Represents the information about the mysterious artifact quest and how it is progressing
		//0 = not started, 1 = started
		storedInformation["mysteriousArtifactStatus"] = 0;

		//Represents the information about the One Minor Task quest and how it is progressing
		//0 = not started, 1 = startedx
		storedInformation["minorTaskStatus"] = 0;
	}
};

#endif
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
		//In general, 0 = false, 1 = true.
		storedInformation["easterComplete"] = 0;
		storedInformation["easterFeatherRequired"] = 0;
		storedInformation["easterEggRequired"] = 0;
	}
};

#endif
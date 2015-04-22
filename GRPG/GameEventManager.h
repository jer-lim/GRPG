// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef GAMEEVENTMGR_H
#define GAMEEVENTMGR_H

#include "constants.h"
#include "GameEvent.h"
#include "QuestCondition.h"
#include "Quest.h"
#include <vector>

class GameEventManager{
private:
	vector<Quest*> vector_listeners;
	UI* ui;
	//Entities to notify on quest change in case they need an update
	vector<Entity*> vector_entities;
	QuestData* questData;
public:
	~GameEventManager(){
		//destruction of listeners are to be done by the quests themselves
		vector_listeners.clear();
	}
	GameEventManager(UI* i, QuestData* qd){
		questData = qd;
		ui = i;
	}
	void addListener(Quest* qc){
		vector_listeners.push_back(qc);
	}
	void addListener(Entity* e)
	{
		vector_entities.push_back(e);
	}
	void removeListener(Quest* qc){
		for (int i = 0, l = vector_listeners.size(); i < l; ++i)
		{
			if (vector_listeners.at(i) == qc)
			{
				vector_listeners.erase(vector_listeners.begin() + i);
				break;
			}
		}
	}
	/**
	GameEvent is deleted after use
	*/
	void informListeners(GameEvent* e){
		bool changedCaused = false;
		for (int i = 0, l = vector_listeners.size(); i < l; ++i)
		{
			int result = vector_listeners.at(i)->eventOccured(e,ui);
			if (result != NO_CHANGE)
			{
				changedCaused = true;
			}
		}
		delete e;
		if (changedCaused)
		{
			for (int i = 0; i < vector_entities.size(); i++)
			{
				vector_entities[i]->questAction(questData, this);
			}
		}
	}
};
#endif
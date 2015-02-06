// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef GAMEEVENTMGR_H
#define GAMEEVENTMGR_H

#include "constants.h"
#include "GameEvent.h"
#include "QuestCondition.h"
#include <vector>

class GameEventManager{
private:
	vector<Quest*> vector_listeners;
	UI* ui;
public:
	~GameEventManager(){
		//destruction of listeners are to be done by the quests themselves
		vector_listeners.clear();
	}
	GameEventManager(UI* i){
		ui = i;
	}
	void addListener(Quest* qc){
		vector_listeners.push_back(qc);
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
		for (int i = 0, l = vector_listeners.size(); i < l; ++i)
		{
			vector_listeners.at(i)->eventOccured(e,ui);
		}
		delete e;
	}
};
#endif
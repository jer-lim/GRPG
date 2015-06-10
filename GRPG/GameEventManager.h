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
#include "NPC.h"
#include "Rift.h"
#include <vector>

class GameEventManager{
private:
	vector<Quest*> vector_listeners;
	UI* ui;
	//Entities to notify on quest change in case they need an update
	vector<Entity*> vector_entities;
	QuestData* questData;
	//Rift needs to know whenever damage is dealt to move on to progress status
	vector<Rift*> vector_rifts;
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
		/*
			//For debugginb
		stringstream ss;
		if (e->getPerson() != nullptr)
			ss << "Entity listener added: " << ((NPC*)e->getPerson())->getname();
		else
			ss << "Entity listener added: " << e->getType();
		ss << " Position: " << vector_entities.size();
		ui->addChatText(ss.str());*/
		if (e->getType() == "RIFT")
		{
			vector_rifts.push_back((Rift*)e);
		}
		else
		{
			vector_entities.push_back(e);
		}
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
	void removeListener(Entity* e)
	{
		if (e->getType() == "RIFT")
		{
			for (int i = 0, l = vector_rifts.size(); i < l; ++i)
			{
				if (vector_rifts.at(i) == (Rift*)e)
				{
					vector_rifts.erase(vector_rifts.begin() + i);
					break;
				}
			}
		}
		else
		{
			for (int i = 0, l = vector_entities.size(); i < l; ++i)
			{
				if (vector_entities.at(i) == e)
				{
					/*
					//For debugging
					stringstream ss;
					ss << "Entity removed at position: " << i;
					ui->addChatText(ss.str());*/
					vector_entities.erase(vector_entities.begin() + i);
					break;
				}
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
		//Inform all rifts
		for (int i = 0, l = vector_rifts.size(); i < l; ++i)
		{
			vector_rifts.at(i)->eventOccured(e, ui);
		}
		delete e;
		if (changedCaused)
		{
			informEntities();
		}
	}

	//Inform all listening entities that a quest update has been performed;
	//and thus it may be required that something relevant happens to them.
	virtual void informEntities(){
		for (int i = 0; i < vector_entities.size(); i++)
		{
			_ASSERTE(_CrtCheckMemory());
			vector_entities[i]->questAction(questData, this);
		}
	}
};
#endif
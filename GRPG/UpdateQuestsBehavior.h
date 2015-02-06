//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef UPDATEQUESTSBEHAVIOR_H
#define UPDATEQUESTSBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
#include "QuestLoader.h"

using namespace std;
class Player;
class UI;
class Grpg;

class UpdateQuestsBehavior :public Behavior
{
private:
	QuestLoader* questLoader;
	Entity* entity;
	Player* player;
	UI* ui;
	Grpg* grpg;
public:
	UpdateQuestsBehavior(Grpg* game,QuestLoader* ql,Entity* e, Player* pl, UI* u){
		grpg = game;
		questLoader = ql;
		entity = e;
		player = pl;
		ui = u;
	}
	virtual ~UpdateQuestsBehavior() {}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif
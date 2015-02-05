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
	Player* player;
	UI* ui;
	Grpg* grpg;
public:
	UpdateQuestsBehavior(Grpg* game,QuestLoader* ql, Player* pl, UI* u){
		grpg = game;
		questLoader = ql;
		player = pl;
		ui = u;
	}
	virtual ~UpdateQuestsBehavior() {}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif
//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef TALKBEHAVIOR_H
#define TALKBEHAVIOR_H

#include "Behavior.h"
#include "ChatData.h"
using namespace std;
class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class UI;
class Player;
class Entity;
class Grpg;
class GameEventManager;
class QuestData;

class TalkBehavior : public Behavior, public OptionNotifier
{
private:
	NPC* ii;
	UI* ui;
	Player* thePlayer;
	Entity* entity; //The entity to talk to
	//For detailed quest data
	Grpg* grpg;
	GameEventManager* gem;
	QuestData* questData;

	void generateEasterQuestTalk(ChatDecision* cd);

public:
	virtual ~TalkBehavior() {}
	TalkBehavior(NPC* i, UI* u, Player* p, Entity* e, Grpg* g);

	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();

	virtual void optionSelected(ChatOption co);
};
#endif
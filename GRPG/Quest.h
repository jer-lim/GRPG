#ifndef _QUEST_H                 // Prevent multiple definitions if this 
#define _QUEST_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "QuestCondition.h"
#include "InventoryItem.h"
//#include "GameEventManager.h"
#include "Button.h"

class GameEventManager;

namespace QuestNS{
	const float INITIAL_DISPLACEMENT = 40;
	const float SIDE_DISPLACEMENT = 10;
	const float MARGIN = 10;
	const float WIDTH = 150;
	const float HEIGHT = 20;
	const DWORD BACK_COLOR = graphicsNS::BLUE;

}

class Quest
{
private:
	string name;
	string description;
	QuestCondition* completeCondition;//beginCondition, completeCondition;
	//bool completed;
	//InventoryItem* reward;//lazy
	int gold;
	Button* ui_element;
public:
	~Quest(){
		SAFE_DELETE(completeCondition);
		SAFE_DELETE(ui_element);	
		//delete reward;//changed to int lol lazy boy ah matthew
	}
	Quest(GameEventManager* qcM, string nama, string descript, QuestCondition* completeCond, int gp, Button* b);
	int eventOccured(GameEvent* ge,UI* ui){
		int result = completeCondition->eventOccured(ge);
		switch (result)
		{
		case QUEST_SUCCESS:
			ui->addChatText("You have made progress in quest " + name + "!");
			break;
		case COMPLETED:
			ui->addChatText("You have completed quest " + name + "!\nProceed to the Quest Board for your reward!");
			break;
		}
		return result;
	}
	string getname(){ return name; }
	void setname(string n){ name = n; }
	string getdescript(){ return description; }
	void setdescript(string n){ description = n; }
	QuestCondition* getQuestCondition() { return completeCondition; }
	void setQuestCondition(QuestCondition* qc) { completeCondition = qc; }
	int getgold(){ return gold; }
	void setgold(int n){ gold = n; }
	Button* getUIElement(){ return ui_element; }
	void setUIElement(Button* b){ ui_element = b; }
};

#endif
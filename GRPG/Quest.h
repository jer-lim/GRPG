#ifndef _QUEST_H                 // Prevent multiple definitions if this 
#define _QUEST_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "QuestCondition.h"
#include "InventoryItem.h"
#include "GameEventManager.h"
#include "Button.h"

namespace QuestNS{
	const float INITIAL_DISPLACEMENT = 30;
	const float MARGIN = 5;
	const float WIDTH = 40;
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
	Quest(GameEventManager* qcM, string nama, string descript, QuestCondition* completeCond, int gp, Button* b)//InventoryItem* prize)
	{
		name = nama;
		description = descript;
		completeCondition = completeCond;
		gold = gp;
		qcM->addListener(completeCond);
		ui_element = b;
		//reward = prize;
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
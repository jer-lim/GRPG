#include "QuestLoader.h"
#include "PersonLoader.h"

void QuestLoader::loadAllQuests(GameEventManager* gem,PersonLoader* personLoader,Graphics* g,float uiX,float uiY){
	ifstream queststream;
	//Misc Items
	queststream.open(questLocation);
	if (queststream.is_open()){
		string name, descript,type;//type cannot be a char, must be a string
		int id, fromID, toID, gold, timesReq;
		while (!queststream.eof()){
			queststream >> id >> name >> descript >> fromID >> toID >> timesReq >> gold >> type;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			//Item* myItem = new Item(name, descript, img_filename, stackcount, cost, itemNS::spriteColForMisc);
			QuestCondition* qc = nullptr;
			Person* from = personLoader->getNPC(fromID);
			if (fromID == -1)
				from = nullptr;
			else
				from = personLoader->getNPC(fromID);
			if (type == "K")
			{
				//kill
				qc = new QuestCondition(new GameEvent_Damage(from, personLoader->getNPC(toID), -1, true), timesReq);
			}
			else if (type == "D")
			{
				//dmg
				qc = new QuestCondition(new GameEvent_Damage(from, personLoader->getNPC(toID), 1, false), timesReq);
			}
			else
			{
				qc = new QuestCondition(new GameEvent(personLoader->getNPC(fromID), personLoader->getNPC(toID)), timesReq);
			}
			Button* b = new Button();
			b->initialize(g, QuestNS::SIDE_DISPLACEMENT+uiX, QuestNS::INITIAL_DISPLACEMENT + uiY + QuestNS::HEIGHT*id + QuestNS::MARGIN*(id - 1), QuestNS::WIDTH, QuestNS::HEIGHT, QuestNS::BACK_COLOR, name);
			descript = String_Functions::translateNewLines(descript);
			Quest* myQuest = new Quest(gem, name, descript, qc, gold,b);
			if (!mapQuests.count(id))
				mapQuests[id] = myQuest;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Quest with same ID " + id));
		}
		queststream.close();
	}
}
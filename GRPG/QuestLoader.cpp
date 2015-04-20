//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

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
				qc = new QuestCondition();
				GameEvent* ge = new GameEvent_Damage(from, personLoader->getNPC(toID), -1, true, "I still need to kill the required number of enemies.", "I have killed the required number of enemies.");
				qc->addGameEventRequirement(ge, timesReq, nullptr);
			}
			else if (type == "D")
			{
				//dmg
				qc = new QuestCondition();
				GameEvent* ge = new GameEvent_Damage(from, personLoader->getNPC(toID), 1, false, "I still need to deal the required amount of damage.", "I have dealt the required amount of damage.");
				qc->addGameEventRequirement(ge, timesReq, nullptr);
			}
			else
			{
				//qc = new QuestCondition(new GameEvent(personLoader->getNPC(fromID), personLoader->getNPC(toID)), timesReq);
			}
			Button* b = new Button();
			b->initialize(g, QuestNS::SIDE_DISPLACEMENT+uiX, QuestNS::INITIAL_DISPLACEMENT + uiY + QuestNS::HEIGHT*id + QuestNS::MARGIN*(id - 1), QuestNS::WIDTH, QuestNS::HEIGHT, QuestNS::BACK_COLOR, name);
			descript = String_Functions::translateNewLines(descript);
			Quest* myQuest = new Quest(gem, name, descript, gold,b);
			myQuest->addQuestCondition(qc);
			if (!mapQuests.count(id))
				mapQuests[id] = myQuest;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Quest with same ID " + id));
		}
		queststream.close();
	}
}
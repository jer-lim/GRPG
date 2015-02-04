#include "QuestLoader.h"
#include "PersonLoader.h"

void QuestLoader::loadAllQuests(GameEventManager* gem,PersonLoader* personLoader){
	ifstream queststream;
	//Misc Items
	queststream.open(questLocation);
	if (queststream.is_open()){
		string name, descript;
		char type;
		int id, fromID, toID, gold, timesReq;
		while (!queststream.eof()){
			queststream >> id >> name >> descript >> fromID >> toID >> timesReq >> gold >> type;
			name = String_Functions::replaceAll(name, '_', ' ');
			descript = String_Functions::replaceAll(descript, '_', ' ');
			//Item* myItem = new Item(name, descript, img_filename, stackcount, cost, itemNS::spriteColForMisc);
			QuestCondition* qc = nullptr;
			switch (type)
			{
				//kill
			case 'K':qc = new QuestCondition(new GameEvent_Damage(personLoader->getNPC(fromID), personLoader->getNPC(toID), -1, true), timesReq);
				break;
				//dmg
			case 'D':qc = new QuestCondition(new GameEvent_Damage(personLoader->getNPC(fromID), personLoader->getNPC(toID), 1, false), timesReq);
				break;
			default: qc = new QuestCondition(new GameEvent(personLoader->getNPC(fromID),personLoader->getNPC(toID)), timesReq);
				break;
			}
			Quest* myQuest = new Quest(gem, name, descript, qc, gold);
			if (!mapQuests.count(id))
				mapQuests[id] = myQuest;
			else
				throw(GameError(gameErrorNS::FATAL_ERROR, "Quest with same ID " + id));
		}
		queststream.close();
	}
}
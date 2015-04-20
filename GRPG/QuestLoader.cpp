//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "QuestLoader.h"
#include "PersonLoader.h"

void QuestLoader::loadAllQuests(GameEventManager* gem,PersonLoader* personLoader,Graphics* g,float uiX,float uiY){
	/*
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
	*/
	//Create an easter quest
	Button* b = new Button();
	b->initialize(g, QuestNS::SIDE_DISPLACEMENT + uiX, QuestNS::INITIAL_DISPLACEMENT + uiY, QuestNS::WIDTH, QuestNS::HEIGHT, QuestNS::BACK_COLOR, "Easter Event 2015");
	Quest* easterQuest = new Quest(gem, "Easter Event 2015", "Help the easter bunny!", 0, b);
	QuestCondition* talkToEasterBunny = new QuestCondition();
	GameEvent* easterBunnyTalked = new GameEvent_EntityAction(nullptr, "I can start this quest by talking to the easter bunny near the Port Sarim Lodestone", "The easter bunny needs help getting his bird to lay eggs after his workers rioted. I need to retrieve 3 items to help them.");
	talkToEasterBunny->addGameEventRequirement(easterBunnyTalked, 1, nullptr);
	easterQuest->addQuestCondition(talkToEasterBunny);

	QuestCondition* fixMachine1 = new QuestCondition();
	GameEvent* getFeathers = new GameEvent_EntityAction(nullptr, "I need to get some feathers to fluff up the bird's nest.", "I managed to acquire some feathers by plucking a chicken.");
	fixMachine1->addGameEventRequirement(getFeathers, 1, nullptr);
	GameEvent* useFeathers = new GameEvent_EntityAction(nullptr, "I need to use these feathers to fix up the machine.", "I used the feathers to fix up the machine.");
	fixMachine1->addGameEventRequirement(useFeathers, 1, getFeathers);
	easterQuest->addQuestCondition(fixMachine1, talkToEasterBunny);
	
	QuestCondition* fixMachine2 = new QuestCondition();
	GameEvent* getEgg = new GameEvent_EntityAction(nullptr, "I need to get some eggs that has been sent out back to act as a sample against further eggs laid.", "I managed to acquire some sample eggs.");
	fixMachine2->addGameEventRequirement(getEgg, 1, nullptr);
	GameEvent* useEgg = new GameEvent_EntityAction(nullptr, "I need to give the eggs to the easter bunny.", "I gave the eggs to the easter bunny.");
	fixMachine2->addGameEventRequirement(useEgg, 1, getEgg);
	easterQuest->addQuestCondition(fixMachine2, talkToEasterBunny);

	QuestCondition* fixMachine3 = new QuestCondition();
	GameEvent* getFood = new GameEvent_EntityAction(nullptr, "I need to get some food and use it to feed the bird.", "I got some food and used them to feed the bird.");
	fixMachine3->addGameEventRequirement(getFood, 1, nullptr);
	easterQuest->addQuestCondition(fixMachine3, talkToEasterBunny);

	QuestCondition* talkToEasterBunnyAgain = new QuestCondition();
	GameEvent* easterBunnyTalk = new GameEvent_EntityAction(nullptr, "I managed to complete all 3 tasks! I should now talk to Easter bunny to see what's next.", "I talked to the easter bunny. He says I now need to soothe the bird.");
	talkToEasterBunnyAgain->addGameEventRequirement(easterBunnyTalk, 1, nullptr);
	vector<QuestCondition*> requiredConditions;
	requiredConditions.push_back(fixMachine1);
	requiredConditions.push_back(fixMachine2);
	requiredConditions.push_back(fixMachine3);
	easterQuest->addQuestCondition(talkToEasterBunnyAgain, requiredConditions);

	QuestCondition* sootheBird = new QuestCondition();
	GameEvent* relaxBird = new GameEvent_EntityAction(nullptr, "", "I managed to soothe the bird, calming him down. I should now go and talk to the easter bunny.");
	sootheBird->addGameEventRequirement(relaxBird, 1, nullptr);
	easterQuest->addQuestCondition(sootheBird, talkToEasterBunnyAgain);

	QuestCondition* talkToEasterBunny3 = new QuestCondition();
	GameEvent* easterBunnyTalk3 = new GameEvent_EntityAction(nullptr, "", "I managed to relax the bird enough and provide a comfortable environment for him to lay eggs! The easter bunny can take care of the rest.");
	talkToEasterBunny3->addGameEventRequirement(easterBunnyTalk3, 1, nullptr);
	easterQuest->addQuestCondition(talkToEasterBunny3, sootheBird);

	mapQuests[0] = easterQuest;
}
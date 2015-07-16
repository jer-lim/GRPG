//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "QuestLoader.h"
#include "PersonLoader.h"
#include "InventoryBoost.h"

void QuestLoader::loadAllQuests(GameEventManager* gem,PersonLoader* personLoader, ItemLoader* itemLoader,Graphics* g,float uiX,float uiY){
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
	Quest* easterQuest = new Quest(gem, questData, "Easter Event 2015", "Help the easter bunny!", b);
	QuestCondition* talkToEasterBunny = new QuestCondition();
	GameEvent* easterBunnyTalked = new GameEvent_EntityAction(personLoader->getNPC(28), "I can start this quest by talking to the easter bunny near the store.", "The easter bunny needs help getting his bird to lay eggs after his workers rioted. I need to retrieve 3 items to help them.");
	//Once quest has started, allow feather plucking
	easterBunnyTalked->addChangeRequired("easterBirdNestStatus", 1);
	easterBunnyTalked->addChangeRequired("easterEggStatus", 1);
	easterBunnyTalked->addChangeRequired("easterStatus", 1);
	talkToEasterBunny->addGameEventRequirement(easterBunnyTalked, 1, nullptr);
	easterQuest->addQuestCondition(talkToEasterBunny);

	QuestCondition* fixMachine1 = new QuestCondition();
	GameEvent* getFeathers = new GameEvent_EntityAction(personLoader->getNPC(15) , "I need to get some feathers to fluff up the bird's nest.", "I managed to acquire some feathers by plucking a chicken.");
	//Once feathers are retrieved, remove need to pluck
	getFeathers->addChangeRequired("easterBirdNestStatus", 2);
	fixMachine1->addGameEventRequirement(getFeathers, 1, nullptr);
	GameEvent* useFeathers = new GameEvent_EntityAction(personLoader->getNPC(29), "I need to use these feathers to fix up the nest.", "I used the feathers to fix up the nest.");
	useFeathers->addChangeRequired("easterBirdNestStatus", 3);
	fixMachine1->addGameEventRequirement(useFeathers, 1, getFeathers);
	easterQuest->addQuestCondition(fixMachine1, talkToEasterBunny);	
	
	QuestCondition* fixMachine2 = new QuestCondition();
	GameEvent* getEgg = new GameEvent_ItemReceived(itemLoader->getItem(31), "I need to get some eggs that has been sent out back to act as a sample against further eggs laid.", "I managed to acquire some sample eggs.");
	getEgg->addChangeRequired("easterEggStatus", 2);
	fixMachine2->addGameEventRequirement(getEgg, 1, nullptr);
	GameEvent* useEgg = new GameEvent_EntityAction(personLoader->getNPC(28), "I need to give the eggs to the easter bunny.", "I gave the eggs to the easter bunny.");
	useEgg->addChangeRequired("easterEggStatus", 3);
	fixMachine2->addGameEventRequirement(useEgg, 1, getEgg);
	easterQuest->addQuestCondition(fixMachine2, talkToEasterBunny);

	QuestCondition* fixMachine3 = new QuestCondition();
	GameEvent* getFood = new GameEvent_ItemUsed(itemLoader->getItem(3), "I need to get some food and use it to feed the bird.", "I got some food and used them to feed the bird.");
	getFood->addChangeRequired("foodGiven", true);
	fixMachine3->addGameEventRequirement(getFood, 1, nullptr);
	easterQuest->addQuestCondition(fixMachine3, talkToEasterBunny);

	QuestCondition* talkToEasterBunnyAgain = new QuestCondition();
	GameEvent* easterBunnyTalk = new GameEvent_EntityAction(personLoader->getNPC(28), "I managed to complete all 3 tasks! I should now talk to Easter bunny to see what's next.", "I talked to the easter bunny. He says I now need to soothe the bird.");
	talkToEasterBunnyAgain->addGameEventRequirement(easterBunnyTalk, 1, nullptr);
	easterBunnyTalk->addChangeRequired("easterStatus", 2);
	vector<QuestCondition*> requiredConditions;
	requiredConditions.push_back(fixMachine1);
	requiredConditions.push_back(fixMachine2);
	requiredConditions.push_back(fixMachine3);
	easterQuest->addQuestCondition(talkToEasterBunnyAgain, requiredConditions);

	QuestCondition* sootheBird = new QuestCondition();
	GameEvent* relaxBird = new GameEvent_EntityAction(personLoader->getNPC(29), "", "I managed to soothe the bird, calming him down. I should now go and talk to the easter bunny.");
	relaxBird->addChangeRequired("easterStatus", 3);
	sootheBird->addGameEventRequirement(relaxBird, 1, nullptr);
	easterQuest->addQuestCondition(sootheBird, talkToEasterBunnyAgain);

	QuestCondition* talkToEasterBunny3 = new QuestCondition();
	GameEvent* easterBunnyTalk3 = new GameEvent_EntityAction(personLoader->getNPC(28), "", "I managed to relax the bird enough and provide a comfortable environment for him to lay eggs! The easter bunny can take care of the rest.");
	talkToEasterBunny3->addGameEventRequirement(easterBunnyTalk3, 1, nullptr);
	easterQuest->addQuestCondition(talkToEasterBunny3, sootheBird);

	easterQuest->addQuestDataChange("easterComplete", true);
	easterQuest->addSkillReward(skillNS::ID_SKILL_STRENGTH, 200);
	easterQuest->addSkillReward(skillNS::ID_SKILL_THIEVING, 300);
	easterQuest->addSkillReward("easterGoodFoodGiven", 1, skillNS::ID_SKILL_COOKING, 200);
	easterQuest->addItemReward(new InventoryFood(itemLoader->getItem(32), 1, DELICIOUS));
	//easterQuest->addItemReward(new InventoryBoost(itemLoader->getItem(33), 1));
	//easterQuest->addItemReward(new InventoryItem(itemLoader->getItem(34), 1));
	//easterQuest->addItemReward(new InventoryFood(itemLoader->getItem(35), 1, DELICIOUS));
	easterQuest->addMiscReward("Monsters throughout GRPG now has a chance to drop easter eggs on death. These eggs can be eaten or traded with the easter bunny.");

	mapQuests[0] = easterQuest;

	Button* b2 = new Button();
	b2->initialize(g, QuestNS::SIDE_DISPLACEMENT + uiX, QuestNS::INITIAL_DISPLACEMENT + uiY + (QuestNS::HEIGHT + QuestNS::MARGIN), QuestNS::WIDTH, QuestNS::HEIGHT, QuestNS::BACK_COLOR, "Stolen Artifact");
	Quest* artifactStealQuest = new Quest(gem, questData, "Stolen Artifact", "An artifact has been stolen by a rich, pompous man. Help to steal it back!", b2);
	QuestCondition* talkToPoorMan = new QuestCondition();
	GameEvent* poorManTalked = new GameEvent_EntityAction(personLoader->getNPC(32), "To start this quest, I can talk to the shriveled looking man in the house just south of the chicken pen.", "The man I talked to claims that an artifact belonging to him has been stolen by a rich, pompous man! I should try to get it back! The rick person's house is located aways east of the doctor.");
	poorManTalked->addChangeRequired("artifactStealStatus", 1);
	talkToPoorMan->addGameEventRequirement(poorManTalked, 1, nullptr);
	artifactStealQuest->addQuestCondition(talkToPoorMan);
	
	QuestCondition* getKey = new QuestCondition(false); //Note: Optional
	GameEvent* keyRetrieved = new GameEvent_ItemReceived(itemLoader->getItem(36), "I could get keys to the house to enter it. The man had some ideas, or I can talk to the gardener just outside the house.", "I managed to get the key to the house!");
	getKey->addGameEventRequirement(keyRetrieved, 1, nullptr);
	keyRetrieved->addChangeRequired("artifactStealKey", 1);
	artifactStealQuest->addQuestCondition(getKey, talkToPoorMan);

	QuestCondition* getIntoHouse = new QuestCondition();
	GameEvent* gotIntoHouse = new GameEvent_ItemUsed(itemLoader->getItem(36), "Alternatively, I could find another way to get into the house without using the key", "I managed to enter the house!");
	getIntoHouse->addGameEventRequirement(gotIntoHouse, 1, nullptr);
	gotIntoHouse->addChangeRequired("artifactStealKey", 1);
	gotIntoHouse->addChangeRequired("artifactStealStatus", 2);
	artifactStealQuest->addQuestCondition(getIntoHouse, talkToPoorMan);

	QuestCondition* getArtifact = new QuestCondition();
	GameEvent* leverPulled = new GameEvent_EntityAction(personLoader->getNPC(36), "I can't get the artifact yet though, something appears to be blocking me.", "I pulled a lever, and the artifact is now within my grasp! ");
	getArtifact->addGameEventRequirement(leverPulled, 1, nullptr);
	leverPulled->addChangeRequired("artifactDisplayStatus", 1);
	GameEvent* artifactAcquired = new GameEvent_ItemReceived(itemLoader->getItem(37), "All I need to do now is to take it!", "Obviously, I took the artifact.");
	getArtifact->addGameEventRequirement(artifactAcquired, 1, leverPulled);
	artifactAcquired->addChangeRequired("artifactStealStatus", 3);
	artifactStealQuest->addQuestCondition(getArtifact, getIntoHouse);

	QuestCondition* talkToPoorMan2 = new QuestCondition();
	GameEvent* poorManTalked2 = new GameEvent_EntityAction(personLoader->getNPC(32), "I should now get back to that man to return the artifact.", "I managed to return the artifact to that man. He acted... somewhat oddly after receiving it, and left... I'm sure there's no way what I just did could ever turn out badly.");
	talkToPoorMan2->addGameEventRequirement(poorManTalked2, 1, nullptr);
	poorManTalked2->addChangeRequired("artifactStealStatus", 4);
	artifactStealQuest->addQuestCondition(talkToPoorMan2, getArtifact);

	artifactStealQuest->addItemReward(new InventoryItem(itemLoader->getItem(0), 500));
	artifactStealQuest->addSkillReward("artifactStealMethod", 1, skillNS::ID_SKILL_FISHING, 200);
	artifactStealQuest->addSkillReward("artifactStealMethod", 2, skillNS::ID_SKILL_THIEVING, 200);
	artifactStealQuest->addSkillReward("artifactStealMethod", 3, skillNS::ID_SKILL_ATTACK, 50);
	artifactStealQuest->addSkillReward("artifactStealMethod", 3, skillNS::ID_SKILL_STRENGTH, 50);
	artifactStealQuest->addSkillReward("artifactStealMethod", 3, skillNS::ID_SKILL_DEFENSE, 50);
	artifactStealQuest->addSkillReward("artifactStealMethod", 3, skillNS::ID_SKILL_TOUGHNESS, 50);
	artifactStealQuest->addSkillReward("artifactStealMethod", 4, skillNS::ID_SKILL_TOUGHNESS, 100);
	artifactStealQuest->addSkillReward("artifactStealMethod", 4, skillNS::ID_SKILL_DEFENSE, 100);

	mapQuests[1] = artifactStealQuest;

	Button* b3 = new Button();
	b3->initialize(g, QuestNS::SIDE_DISPLACEMENT + uiX, QuestNS::INITIAL_DISPLACEMENT + uiY + 2 * (QuestNS::HEIGHT + QuestNS::MARGIN), QuestNS::WIDTH, QuestNS::HEIGHT, QuestNS::BACK_COLOR, "Mysterious Artifact");
	Quest* mysteriousArtifactQuest = new Quest(gem, questData, "Mysterious artifact", "The artifact has a more back rich history than it originally seems...", b3);
	QuestCondition* talkToGardener = new QuestCondition();
	GameEvent* gardenerTalked = new GameEvent_EntityAction(personLoader->getNPC(37), "To start this quest, I can talk to the gardener, Alfred, right outside the house located east of the doctor. I need to have completed the Stolen Artifact Quest to start this quest.", "The artifact I stole may not have actually belonged to the poor man. Alfred needs some help and asked me to run a few errands for him.");
	gardenerTalked->addChangeRequired("mysteriousArtifactStatus", 1);
	talkToGardener->addGameEventRequirement(gardenerTalked, 1, nullptr);
	mysteriousArtifactQuest->addQuestCondition(talkToGardener);

	QuestCondition* completeGardenerTask = new QuestCondition();
	completeGardenerTask->setRequired(false);
	GameEvent* pickATask = new GameEvent_EntityAction(personLoader->getNPC(37), "I should talk with Alfred to choose a task to start on first.", "I've chosen a task to start on.");
	completeGardenerTask->addGameEventRequirement(pickATask, 1, nullptr);
	GameEvent* completeTask = new GameEvent_EntityAction(nullptr, "I should work on that. Alfred can remind me on what task needs doing if I've forgotten.", "I've managed to complete the task Alfred assigned me!");
	completeGardenerTask->addGameEventRequirement(completeTask, 1, pickATask);
	mysteriousArtifactQuest->addQuestCondition(completeGardenerTask, talkToGardener);

	QuestCondition* openTheRift = new QuestCondition();
	GameEvent* openRift = new GameEvent_EntityAction(personLoader->getNPC(39), "", "While leaving the garden, a rift opened up right in front of me!");
	openRift->addChangeRequired("mysteriousArtifactStatus", 3);
	openTheRift->addGameEventRequirement(openRift, 1, nullptr);
	mysteriousArtifactQuest->addQuestCondition(openTheRift, talkToGardener);

	QuestCondition* enterRift = new QuestCondition();
	GameEvent* talkToAlfred = new GameEvent_EntityAction(personLoader->getNPC(37), "Alfred seems to know what's up with that - I should go and talk to him.", "Alfred says there's no time to explain.");
	talkToAlfred->addChangeRequired("mysteriousArtifactStatus", 4);
	enterRift->addGameEventRequirement(talkToAlfred, 1, nullptr);
	GameEvent* enterTheRift = new GameEvent_EntityAction(personLoader->getNPC(39), "I should enter the rift!", "I entered the rift. Now to find out what's going on.");
	enterRift->addGameEventRequirement(enterTheRift, 1, talkToAlfred);
	enterTheRift->addChangeRequired("mysteriousArtifactStatus", 5);
	mysteriousArtifactQuest->addQuestCondition(enterRift, openTheRift);

	QuestCondition* closeTheRift = new QuestCondition();
	GameEvent* startRift = new GameEvent_EntityAction(personLoader->getNPC(32), "I found the shriveled man in the rift. I should go talk to him.", "The shriveled man turned out to be causing the rift! He sent a bunch of monsters against me! ");
	startRift->addChangeRequired("mysteriousArtifactStatus", 6);
	closeTheRift->addGameEventRequirement(startRift, 1, nullptr);
	GameEvent* exitRift = new GameEvent_RiftClosed(nullptr, "I should fight off the monsters as much as possible, exiting the rift when I cannot handle any more.", "After some fights, I exited the rift, out of the dark realm. The rift closed behind me.");
	closeTheRift->addGameEventRequirement(exitRift, 1, startRift);
	exitRift->addChangeRequired("mysteriousArtifactStatus", 7);
	mysteriousArtifactQuest->addQuestCondition(closeTheRift, enterRift);

	QuestCondition* getAnExplanation = new QuestCondition();
	GameEvent* getExplanation = new GameEvent_EntityAction(personLoader->getNPC(40), "The owner of the mysterious artifact appears to be here. It's time to find out what's going on.", "After talking to Vangel, I learned some information about what happened. I can talk to him again to recap that information. He has sent his scouts to learn more, and will let me know if my help is required.");
	getExplanation->addChangeRequired("mysteriousArtifactStatus", 8);
	getAnExplanation->addGameEventRequirement(getExplanation, 1, nullptr);
	mysteriousArtifactQuest->addQuestCondition(getAnExplanation, closeTheRift);

	mysteriousArtifactQuest->addItemReward(new InventoryEquipment(itemLoader->getItem(14), 1, (Smithing_Material*)itemLoader->getItem(7)));
	mysteriousArtifactQuest->addMiscReward("Rifts will now appear throughout GRPG above the wilderness line.");

	mapQuests[2] = mysteriousArtifactQuest;

	Button* b4 = new Button();
	b4->initialize(g, QuestNS::SIDE_DISPLACEMENT + uiX, QuestNS::INITIAL_DISPLACEMENT + uiY + 3 * (QuestNS::HEIGHT + QuestNS::MARGIN), QuestNS::WIDTH, QuestNS::HEIGHT, QuestNS::BACK_COLOR, "One Small Favour");
	Quest* oneMinorTask = new Quest(gem, questData, "One Small Favour", "This quest's name is really familiar. It's bound to be as annoying.", b4);
	QuestCondition* getQuest = new QuestCondition();
	GameEvent* captainTalked = new GameEvent_EntityAction(personLoader->getNPC(24), "I can start this quest, which is probably going to be really annoying, by talking to Captain Point of No Return. He can be found at the east side of the northen edge of the southern river.", "Captain Point of No Return wants me to do a minor task for him. This probably isn't as simple as it seems; I know how these sort of quests turned out. ");
	getQuest->addGameEventRequirement(captainTalked, 1, nullptr);
	GameEvent* acceptQuest = new GameEvent_ItemReceived(itemLoader->getItem(39), "I'm not convinced I should waste so much of my time to help him.", "I really shouldn't have agreed to it, but he seemed so pitiful.");
	acceptQuest->addChangeRequired("minorTaskStatus", 1);
	getQuest->addGameEventRequirement(acceptQuest, 1, captainTalked);
	oneMinorTask->addQuestCondition(getQuest);

	QuestCondition* talkToFisherman = new QuestCondition();
	GameEvent* fishermanTalked = new GameEvent_EntityAction(personLoader->getNPC(44), "I should talk to the fisherman to get the his net fixed.", "I talked with the fisherman. As expected, he was not willing to just let this quest be short, but instead asked for another favour. This is going to be a long quest.");
	talkToFisherman->addGameEventRequirement(fishermanTalked, 1, nullptr);
	fishermanTalked->addChangeRequired("minorTaskStatus", 2);
	oneMinorTask->addQuestCondition(talkToFisherman, getQuest);

	QuestCondition* complainAboutQuest = new QuestCondition();
	complainAboutQuest->setRequired(false);
	GameEvent* captainComplained = new GameEvent_EntityAction(personLoader->getNPC(24), "", "I took a short detour and complained to Captain Point of No Return about this quest. He is now willing to teach me about fishing if I can continue helping the fisherman as well.");
	complainAboutQuest->addGameEventRequirement(captainComplained, 1, nullptr);
	captainComplained->addChangeRequired("minorTaskComplained", 1);
	oneMinorTask->addQuestCondition(complainAboutQuest, talkToFisherman);

	QuestCondition* solveFishermanIllness = new QuestCondition();
	GameEvent* doctorTalked = new GameEvent_EntityAction(personLoader->getNPC(0), "I should talk with the doctor to see about the fisherman's problems.", "The doctor didn't ask for another favour, but went to see the fisherman immediately.");
	solveFishermanIllness->addGameEventRequirement(doctorTalked, 1, nullptr);
	doctorTalked->addChangeRequired("minorTaskStatus", 3);
	GameEvent* fishermanTalked2 = new GameEvent_EntityAction(personLoader->getNPC(44), "I expect to be asked to do another favour once he figures out he cannot solve the fisherman's problems and needs something.", "This defies all logic. I expected him to ask for another favour, but he didn't! He solved the fisherman's illness and the fisherman was able to fix the net for me!");
	solveFishermanIllness->addGameEventRequirement(fishermanTalked2, 1, doctorTalked);
	fishermanTalked2->addChangeRequired("minorTaskStatus", 4);
	oneMinorTask->addQuestCondition(solveFishermanIllness, talkToFisherman);

	QuestCondition* finishQuest = new QuestCondition();
	GameEvent* captainTalked2 = new GameEvent_ItemUsed(itemLoader->getItem(39), "I can now return the net to the captain.", "I returned to the net to the captain, and completed the quest. This quest turned out to be significantly less annoying than I originally anticipated.");
	finishQuest->addGameEventRequirement(captainTalked2, 1, nullptr);
	captainTalked2->addChangeRequired("minorTaskStatus", 5);
	oneMinorTask->addQuestCondition(finishQuest, solveFishermanIllness);

	oneMinorTask->addSkillReward("minorTaskComplained", 1, skillNS::ID_SKILL_FISHING, 200);
	oneMinorTask->addMiscReward("The captain will tell you more on how Aildiuln fights.");

	mapQuests[3] = oneMinorTask;
	
	//Create quest "Breath of Fresh Air"
	//KIV atm
	/*
	Button* b5 = new Button();
	b5->initialize(g, QuestNS::SIDE_DISPLACEMENT + uiX, QuestNS::INITIAL_DISPLACEMENT + uiY + 4*(QuestNS::HEIGHT + QuestNS::MARGIN), QuestNS::WIDTH, QuestNS::HEIGHT, QuestNS::BACK_COLOR, "Breath of Fresh Air");
	Quest* smokeQuest = new Quest(gem, questData, "Breath of Fresh Air", "Help Nole figure out what his problem is and solve it", b5);
	QuestCondition* talkToNoleMom = new QuestCondition();
	GameEvent* noleMomTalked = new GameEvent_EntityAction(nullptr, "To start this quest, I can talk to Nole's mom in the house just east of the doctor.", "I have talked with Nole's mom. She's worried about her son, Nole, and wonders if anything is troubling him.");
	noleMomTalked->addChangeRequired("smokeStatus", 1);
	talkToNoleMom->addGameEventRequirement(noleMomTalked,1,nullptr);
	smokeQuest->addQuestCondition(talkToNoleMom);

	QuestCondition* talkToNole = new QuestCondition();
	GameEvent* noleTalked = new GameEvent_EntityAction(nullptr, "Maybe I should go and talk with him.", "I talked with Nole. After some time, he had to go for a smoke, but revealed that he was unhappy about constantly getting 4th. Maybe I can do something about it.");
	noleTalked->addChangeRequired("elliotStatus", 1);
	noleTalked->addChangeRequired("geraldStatus", 1);
	noleTalked->addChangeRequired("mattStatus", 1);
	talkToNole->addGameEventRequirement(noleTalked, 1, nullptr);
	smokeQuest->addQuestCondition(talkToNole, talkToNoleMom);

	mapQuests[1] = smokeQuest;*/
}
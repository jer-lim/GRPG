//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "talkBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "grpg.h"
#include "ShriveledMan.h"
#include "InventoryItem.h"
#include "mapLoader.h"
#include "PersonLoader.h"

TalkBehavior::TalkBehavior(NPC* i, UI* u, Player* p, Entity* e, Grpg* g){
	ii = i;
	ui = u;
	thePlayer = p;
	entity = e;
	grpg = g;
	gem = g->getGameEventManager();
	questData = g->getQuestLoader()->getQuestData();
}

string TalkBehavior::displayText(){ return "Talk to " + ii->getname(); }
void TalkBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*thePlayer, collisionVector))
	{
		if (ii->getTalkText().find("detail") == 0)
		{
			ui->removeWindow();
			int detailNumber = stoi(ii->getTalkText().substr(6));
			switch (detailNumber)
			{
				case 1:
				{
					ui->drawWindow("Easter Bunny");
					if (questData->getValue("easterStatus") == 0)
					{
						ui->addTalkText(new ChatInformation("Hello, I'm the easter bunny!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("I generally bring easter to the entire world, helping to bring joy through easter eggs to everyone - even the ones that tend to be hostile! I don't show bias to anyone!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("Unfortunately, it doesn't seem like there'll be easter today.", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(1, "Why, what's the matter?");
						dt->addOption(2, "Wow, it's the easter bunny! That's amazing!");
						dt->addOption(0, "Uh, I should go. Things to do...");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else if (questData->getValue("easterComplete"))
					{
						ui->addTalkText(new ChatInformation("Hey! Thanks for all your help!", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(6, "About those items you required earlier...");
						dt->addOption(15, "About these easter eggs...");
						dt->addOption(0, "Gotta go, see ya");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else if (questData->getValue("easterStatus") == 3)
					{
						ui->addTalkText(new ChatInformation("I've managed to soothe the bird!", chatNS::LEFT));
						ui->addTalkText(new ChatInformation("So I see. Great job! You've been a big help in getting easter started!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("So easter can continue?", chatNS::LEFT));
						ui->addTalkText(new ChatInformation("That's right! Thank you for all your help!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("No problem!", chatNS::LEFT));
						ui->addTalkText(new ChatInformation("As a reward, here's a common easter egg for you! Also, you may be able to find some more easter eggs as you kill monsters, since they will also be getting easter eggs and may drop them once killed. You can use these easter eggs for yourself or sell them to me.", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(0, "Thanks!");
						dt->setCaller(this);
						ui->addTalkText(dt);
						gem->informListeners(new GameEvent_EntityAction(ii));
					}
					else if (questData->getValue("foodGiven") && questData->getValue("easterBirdNestStatus") == 3 && questData->getValue("easterEggStatus") == 3)
					{
						ui->addTalkText(new ChatInformation("Hello again!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("I've completed all the tasks you've asked me to do!", chatNS::LEFT));
						ui->addTalkText(new ChatInformation("Great job! However, the bird's still looking a bit restless. Perhaps you could help me soothe the bird?", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("Soothe the bird?", chatNS::LEFT));
						ui->addTalkText(new ChatInformation("Yes, you know, say kind words to it, make it feel better, pet it and stuff. I'm not too good and doing that, maybe you could help me?", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(0, "Alright");
						dt->setCaller(this);
						ui->addTalkText(dt);
						gem->informListeners(new GameEvent_EntityAction(ii));
					}
					else
					{
						ui->addTalkText(new ChatInformation("Hello again!", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->setCaller(this);
						generateEasterQuestTalk(dt);
						ui->addTalkText(dt);
					}
					break;
				}
				case 2:
				{
					ui->drawWindow("Shriveled Man");
					if (questData->getValue("artifactStealStatus") == 0)
					{
						ui->addTalkText(new ChatInformation("Good day to you.", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->setCaller(this);
						dt->addOption(20, "Do you have a quest for me?");
						dt->addOption(0, "Good day to you, too. See you around!");
						ui->addTalkText(dt);
					}
					else if (questData->getValue("artifactStealStatus") == 1)
					{
						ui->addTalkText(new ChatInformation("Thank you for agreeing to help me!", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(23, "I need some information on how to get into the house");
						dt->addOption(0, "Oh yeah, gotta go help you.");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else if (questData->getValue("artifactStealStatus") == 2)// door opened
					{
						ui->addTalkText(new ChatInformation("I've managed to open the door, gaining access to the house!", chatNS::LEFT));
						ui->addTalkText(new ChatInformation("Excellent! So, do you have the artifact with you?", chatNS::RIGHT));
						if (questData->getValue("artifactDisplayStatus") == 0)//Display case closed
						{
							ui->addTalkText(new ChatInformation("No. I couldn't get to it, there appears to be some barrier protecting the artifact.", chatNS::LEFT));
							ui->addTalkText(new ChatInformation("Hmph. Well, try looking about near the artifact, there's bound to be something that deactivates it. Be sure to examine everything, some are not revealed at first glance.", chatNS::RIGHT));
						}
						else
						{
							ui->addTalkText(new ChatInformation("Well, I found the artifact locked behind a barrier, but I managed to pull a lever to unlock it. I haven't taken it yet though.", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("Well, what are you waiting for? Go take it!", chatNS::RIGHT));
						}
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(0, "All right.");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else if (questData->getValue("artifactStealStatus") == 3)
					{
						ui->addTalkText(new ChatInformation("Look! I managed to get the artifact for you!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("Awesome! Thank you, now give it here to me.", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("You hand the artifact over.", chatNS::MIDDLE));
						ui->addTalkText(new ChatInformation("Thank You! Now, with this artifact, I will be able to ...", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("The man seems to realize you are still here.", chatNS::MIDDLE));
						ui->addTalkText(new ChatInformation("Uh, well, place it back in my, um, house, for protection and never use or touch again. Yes, that's what I'll do. Nothing evil or bad, definitely not.", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(33, "Um, are you ok?");
						dt->addOption(34, "That's great and all, now how about a reward?");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else if (questData->getValue("artifactStealStatus") == 4)
					{
						//Mysterious artifact quest rift section
						if (questData->getValue("mysteriousArtifactStatus") == 5)
						{
							ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
							dt->addOption(58, "What is this place?");
							dt->addOption(59, "What's going on in here?");
							dt->addOption(60, "What have you done?");
							dt->setCaller(this);
							ui->addTalkText(dt);
						}
						else //Somehow talking, either by completing stolen artifact through console commands
							//or by closing the conversation window without properly ending it.
						{
							//Leave
							ui->removeWindow();
							((ShriveledMan*)entity)->startStolenArtifactRun();
						}
					}
					break;
				}
				case 3:
				{
					ui->drawWindow("Gardener");
					ui->addTalkText(new ChatInformation("Hello there!", chatNS::RIGHT));
					ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
					dt->addOption(27, "What do you do here?");
					dt->addOption(29, "Are you allowed to enter the house?");
					if (questData->getValue("artifactStealStatus") == 1)
					{
						dt->addOption(30, "Can I borrow the key to the house?");
					}
					dt->setCaller(this);
					ui->addTalkText(dt);
					break;
				}
				case 4:
				{
					ui->drawWindow("Alfred");
					if (questData->getValue("mysteriousArtifactStatus") <= 2)
					{
						ui->addTalkText(new ChatInformation("Good day, Sir.", chatNS::RIGHT));
						if (questData->getValue("mysteriousArtifactGardenerTask") == 1)
						{
							ui->addTalkText(new ChatInformation("Thanks for agreeing to help me grow some flowers!", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("Remember, just rake the weeds off the flower patch, dig a hole, and then plant the seeds.", chatNS::RIGHT));
						}
						else if (questData->getValue("mysteriousArtifactGardenerTask") == 2)
						{
							ui->addTalkText(new ChatInformation("Thanks for agreeing to help me plant the tree!", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("Remember, just plant the seeds into the plant pot, water it, wait for it to grow, and then place them into the plant patch", chatNS::RIGHT));
						}
						else if (questData->getValue("mysteriousArtifactGardenerTask") == 3)
						{
							ui->addTalkText(new ChatInformation("Thanks for agreeing to help me add some live fish into the pond!", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("Unfortunately, like I said, I have no idea how to do it yet. I'm sure you'll think of something though!", chatNS::RIGHT));
						}
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(36, "Who are you?");
						dt->addOption(37, "What happened to the previous guy?");
						if (questData->getValue("mysteriousArtifactStatus") == 0)
						{
							dt->addOption(39, "Do you need my help?");
						}
						else
						{
							dt->addOption(44, "Tell me about the tasks that needed doing.");
							if (questData->getValue("mysteriousArtifactWaitTime") == 1)
							{
								dt->addOption(53, "I managed to plant the flower seeds, now what?");
							}
							else if (questData->getValue("mysteriousArtifactWaitTime") == 2)
							{
								dt->addOption(53, "I managed to plant the tree seed into the pot, now what?");
							}
							else if (questData->getValue("mysteriousArtifactWaitTime") == 3)
							{
								dt->addOption(53, "I managed to transfer the young tree into the patch, now what?");
							}
						}
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else if (questData->getValue("mysteriousArtifactStatus") < 7)
					{
						ui->addTalkText(new ChatInformation("Look at that - it's a rift!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("Whoever stole the artifact must have created it!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("There's no time to explain - you're the only one nearby! Quick, prepare yourself for combat and jump inside!", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("You need to seal the rift ASAP!", chatNS::RIGHT));
						gem->informListeners(new GameEvent_EntityAction(ii));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(0, "I'm on it!");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else if (questData->getValue("mysteriousArtifactStatus") == 7)
					{
						ui->addTalkText(new ChatInformation("Great job, you managed to close the rift!", chatNS::RIGHT));
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						if (questData->getValue("artifactStealAdmitted") == 1)
						{
							ui->addTalkText(new ChatInformation("You know, when you admitted you stole the artifact I didn't believe you, but now I think you might be telling the truth.", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("Humanity has one redeeming trait - we learn from our mistakes. While what you did previously was wrong, especially when breaking into someone's else house, at least you're trying to change it.", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("You have a long way to go though, what you did has disastrous consequences.", chatNS::RIGHT));
							dt->addOption(0, "Wise words. I hope to be able to correct this mistake.");
						}
						else
						{
							ui->addTalkText(new ChatInformation("Nice work there - I didn't think you would make it.", chatNS::RIGHT));
						}
						ui->addTalkText(new ChatInformation("The owner of the artifact is here now, you should go and speak with him", chatNS::RIGHT));
						gem->informListeners(new GameEvent_EntityAction(ii));
						dt->addOption(0, "All right.");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					else
					{
						if (questData->getValue("mysteriousArtifactAlfredReward") == 0)
						{
							if (questData->getValue("mysteriousArtifactGardenerTask") == 1)
							{
								ui->addTalkText(new ChatInformation("Hi there! About the flowers you planted - thanks for that!", chatNS::RIGHT));
								ui->addTalkText(new ChatInformation("I think you have bigger stuff to deal with now, so let's finish this task. Here's your reward.", chatNS::RIGHT));
								ui->addTalkText(new ChatInformation("Alfred hands you 100 gold.", chatNS::MIDDLE));
								//Give the player 100 gold
								Entity* $$$ = new Entity();
								$$$->initialize(grpg, new InventoryItem(grpg->getItemLoader()->getItem(0), 100));
								grpg->getPlayer()->getInventory()->addEntityInventoryItem($$$, grpg);
								questData->setValue("mysteriousArtifactAlfredReward", 1);
							}
							else if (questData->getValue("mysteriousArtifactGardenerTask") == 2)
							{
								if (questData->getValue("mysteriousArtifactWaitTime") == 2)
								{
									ui->addTalkText(new ChatInformation("Hi there! About the plant pot you have - it's probably grown into a sprout by now. That tends to happen fairly quickly.", chatNS::RIGHT));
									ui->addTalkText(new ChatInformation("Let's finish this task, all right? Transfer the sprout over to the plant patch over there.", chatNS::RIGHT));
								}
								else
								{
									ui->addTalkText(new ChatInformation("Thanks for planting the tree into the patch for me.", chatNS::RIGHT));
									ui->addTalkText(new ChatInformation("Here's your reward for that task", chatNS::RIGHT));
									ui->addTalkText(new ChatInformation("Alfred hands you 100 gold", chatNS::MIDDLE));
									//Give the player 100 gold
									Entity* $$$ = new Entity();
									$$$->initialize(grpg, new InventoryItem(grpg->getItemLoader()->getItem(0), 100));
									grpg->getPlayer()->getInventory()->addEntityInventoryItem($$$, grpg);
									questData->setValue("mysteriousArtifactAlfredReward", 1);
									//Remove the pot from his inventory
									Entity* pot = new Entity();
									pot->initialize(grpg, new InventoryItem(grpg->getItemLoader()->getItem(38), 1));
									vector<Entity*>* removedItems = new vector<Entity*>;
									grpg->getPlayer()->getInventory()->removeEntityInventoryItems(pot, true, removedItems, grpg);
									delete pot;
									for (int i = 0; i < removedItems->size(); i++)
									{
										delete removedItems->at(i);
									}
									removedItems->clear();
									delete removedItems;
								}
							}
							else if (questData->getValue("mysteriousArtifactGardenerTask") == 3)
							{
								ui->addTalkText(new ChatInformation("Hi there! About the tasks with the fishes - forget about it! I think you probably have more important stuff to deal with right now.", chatNS::RIGHT));
								questData->setValue("mysteriousArtifactAlfredReward", 1);
							}
						}
						else
						{
							ui->addTalkText(new ChatInformation("Hi there!", chatNS::RIGHT));
						}
						ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
						dt->addOption(67, "What about your other tasks?");
						dt->addOption(68, "What do you know about the artifact?");
						dt->addOption(0, "Leave");
						dt->setCaller(this);
						ui->addTalkText(dt);
					}
					break;
				}
				case 5:
				{
					ui->drawWindow("Vangel");
					ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
					dt->addOption(63, "What was that place?");
					dt->addOption(64, "How did that portal appear?");
					if (questData->getValue("mysteriousArtifactStatus") == 7)
					{
						ui->addTalkText(new ChatInformation("Well done, you managed to escape.", chatNS::RIGHT));
						if (questData->getValue("artifactStealAdmitted") == 1)
						{
							ui->addTalkText(new ChatInformation("Alfred tells me it was you who stole the artifact from me. I hope you know what the consequences are.", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("It is a good thing that you are now helping to amend that.", chatNS::RIGHT));
						}
						else
						{
							ui->addTalkText(new ChatInformation("That portal was created by a person who stole a previous artifact from my house.", chatNS::RIGHT));
							ui->addTalkText(new ChatInformation("I am glad that there are heroes like you who are attempting to stop these people.", chatNS::RIGHT));
							dt->addOption(62, "Actually, I was the one whole stole the artifact.");
						}
						dt->addOption(65, "Not at the moment.");
					}
					else
					{
						ui->addTalkText(new ChatInformation("My scouts still have not reported back with anything useful.", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("Rest assured, if I get any useful information, I'll inform you immediately.", chatNS::RIGHT));
						dt->addOption(69, "What happens when I enter a rift?");
						dt->addOption(70, "What should I expect when I enter a rift?");
						dt->addOption(0, "Not at the moment");
					}
					ui->addTalkText(new ChatInformation("Do you have any questions for me?", chatNS::RIGHT));
					dt->setCaller(this);
					ui->addTalkText(dt);
					break;
				}
				case 6:
				{
					ui->drawWindow("Fisherman");
					ui->addTalkText(new ChatInformation("Is there anything you would like to know about fishing?", chatNS::RIGHT));
					ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
					dt->addOption(75, "How do I fish?");
					dt->addOption(76, "How do I get better at fishing?");
					dt->addOption(77, "What can I get from fishing?");
					if (questData->getValue("minorTaskStatus") == 1)
					{
						dt->addOption(91, "Could you help me fix this net?");
					}
					else if (questData->getValue("minorTaskStatus") == 3)
					{
						dt->addOption(98, "How are you feeling now?");
					}
					dt->addOption(0, "I already know everything I need to know.");
					dt->setCaller(this);
					ui->addTalkText(dt);
					break;
				}
				case 7:
				{	
					ui->drawWindow("Captain Point of No Return");
					ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
					dt->addOption(79, "I'm ready. Bring me to that island!");
					dt->addOption(80, "Tell me more about the dragon.");
					ui->addTalkText(new ChatInformation("Ahoy! To the south is Ideal Island, where a great and powerful dragon lives. Make sure you're ready to face him, for there is no turning back. Are you sure you want to go there?", chatNS::RIGHT));
					if (questData->getValue("minorTaskStatus") == 0)
					{
						ui->addTalkText(new ChatInformation("Hang on. Say, you look like an adventurer. Could you do me a favour? Please? I need it back urgently.", chatNS::RIGHT));
						dt->addOption(81, "You need a favour?");
					}
					else if (questData->getValue("minorTaskStatus") < 5)
					{
						dt->addOption(90, "About your favour...");
					}
					dt->setCaller(this);
					ui->addTalkText(dt);
					break;
				}
				case 8: 
				{
					ui->drawWindow("Doctor");
					ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
					dt->setCaller(this);
					if (questData->getValue("minorTaskStatus") == 3) //Exception for one minor task
					{
						ui->addTalkText(new ChatInformation("Hey there. The fisherman should be all better now.", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("He wants to talk to you - you should go see what he wants.", chatNS::RIGHT));
						ui->addTalkText(new ChatInformation("I'll be going back now.", chatNS::RIGHT));
						entity->setDestination(new Point(grpg->getMapLoader()->translateIdToCoords('+')));
						dt->addOption(0, "You didn't ask for a favour? Thanks!");
						ui->addTalkText(dt);
						return;
					}
					ui->addTalkText(new ChatInformation("Hello! I can heal you!", chatNS::RIGHT));
					dt->addOption(93, "Please do.");
					dt->addOption(94, "How do you heal me like that?");
					if (questData->getValue("saveCrystalDoctorAsked") == 0)
					{
						dt->addOption(106, "What's up with that shiny crystal?");
					}
					if (questData->getValue("minorTaskStatus") == 2)
					{
						dt->addOption(97, "The fisherman says he needs some help.");
					}
					ui->addTalkText(dt);
					break;
				}
				case 9:
				{
					ui->drawWindow("Lawyer");
					ui->addTalkText(new ChatInformation("Hello, I'm the lawyer. I'm here to credit the relevant people whose images, music and sound are used in this game, so that the creators don't end up getting sued.", chatNS::RIGHT));
					ui->addTalkText(new ChatInformation("Is there anything you would like to know?", chatNS::RIGHT));
					ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
					dt->setCaller(this);
					dt->addOption(109, "Tell me about the images");
					dt->addOption(110, "Tell me about the sound");
					dt->addOption(111, "Tell me about the music");
					dt->addOption(0, "Nothing else, thanks");
					ui->addTalkText(dt);
					break;
				}
				default:
				{
					ui->addChatText("Developer warning: detail chat was required, but");
					stringstream ss;
					ss << "the following detail number was not found: " << detailNumber;
					ui->addChatText(ss.str());
					break;
				}
			}
		}
		else
		{
			ui->addChatText(ii->getname() + ": " + ii->getTalkText());
		}
	}
	else
	{
		thePlayer->setVictim(entity);
		thePlayer->setNPCAction(this);
	}
}

void TalkBehavior::optionSelected(ChatOption co)
{
	ui->addTalkText(new ChatInformation(co.text, chatNS::LEFT));
	//Note to developers: This method will already initalize the final ChatDecision* here for you. Use it.
	//Caller has been set with a default displayType of VERTICALLY, call the set codes to change it if required
	//If not used, please call delete cd; to ensure that there are no leaks.
	ChatDecision* cd = new ChatDecision(chatNS::VERTICALLY);
	cd->setCaller(this);
	switch (co.id)
	{
	case 0: //Always exit
		ui->removeWindow();
		//No use with cd
		delete cd;
		grpg->attemptQuestCompletions();
		//Spawn a rift during the relevant quest
		if (questData->getValue("mysteriousArtifactStatus") == 2)
		{
			gem->informListeners(new GameEvent_EntityAction(grpg->getPersonLoader()->getNPC(39)));
			VECTOR2 riftLocation = VECTOR2(thePlayer->getX(), thePlayer->getY());
			riftLocation.x += GAME_WIDTH / 4;
			NPC::spawn(grpg, 39, riftLocation);
			entity->sayMessage("Whoa - what's that?");
			//Move Alfred to next to the rift
			Point* nextToRift = new Point(riftLocation);
			nextToRift->setX(nextToRift->getX() - ii->getWidth());
			nextToRift->setY(nextToRift->getY() + ii->getHeight());
			entity->releaseDestination();
			entity->setDestination(nextToRift);
			//Change spawn location so now it wanders around the portal instead of returning
			entity->setSpawnPoint(riftLocation);
		}
		break;
	case 1: //Easter: Asks what's the matter
		ui->addTalkText(new ChatInformation("Well...it's a long story.", chatNS::RIGHT));
		cd->addOption(3, "I have time.");
		cd->addOption(4, "Give me a summary of the story then.");
		cd->addOption(5, "Skip it, tell me what you want");
		cd->addOption(0, "I'm kind of busy at the moment, no time. Gotta go!");
		ui->addTalkText(cd);
		break;
	case 2: //Easter: Asks OMG is the easter bunny
		ui->addTalkText(new ChatInformation("Yes! I bring easter to the entire world, giving out easter eggs!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Unfortunately, I've had a bit of a problem, so I'm afraid there won't be Easter this year. Maybe even forever.", chatNS::RIGHT));
		cd->addOption(1, "Why, what's the matter?");
		cd->addOption(0, "Uh, I should go. Things to do...");
		ui->addTalkText(cd);
		break;
	case 3: //Easter: Start long speech
		//Split into 2 parts here so that it's easier to read
		//In the middle of this speech the player is prompted for a decision, and both speeches lead back to id 3
		//As such, text is checked here to find out which part to start talking
		if (co.text != "Go on..." && co.text != "A vacuum cleaner? Can't you just head out and buy some?")
		{
			ui->addTalkText(new ChatInformation("Well, I have a magical bird that lays the easter eggs for me to give them out. I keep it healthy and fed, and it lays the eggs.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("Unfortunately, a few months ago my workers decided to riot, due to Civ not having what they want.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("Who's Civ?", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("Civ is a person who comes by to our hidden base every year, offering goods to trade with us.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("He offers amazing items that you take for granted in your world. For example, my workers were expecting him to have a vacuum cleaner to trade, but he didn't.", chatNS::RIGHT));
			cd->addOption(3, "Go on...");
			cd->addOption(3, "A vacuum cleaner? Can't you just head out and buy some?");
			ui->addTalkText(cd);
		}
		else
		{
			if (co.text == "A vacuum cleaner? Can't you just head out and buy some?")
			{
				ui->addTalkText(new ChatInformation("We could, but it would reveal us, and we don't want to do that too often", chatNS::RIGHT));
			}
			ui->addTalkText(new ChatInformation("The workers really wanted the vacuum cleaner because it would help them automatically clean dust and place them into the bag, instead of them having to manually do it themselves.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("Many of them even prepared specially for it, modifying their homes with available electrical sockets.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("However, it turned out that Civ did not offer that item. The workers rioted with this, claiming ridiculous stuff like 'We work, we say!' or something similar.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("Sounds terrible!", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("It was. They blocked off the bird's food storage, annoyed it, and messed up it's nest.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("Now the bird is too stressed to lay eggs, and unless it does so there won't be easter this year, maybe any more!", chatNS::RIGHT));
			cd->addOption(5, "So, what do you need me to do?");
			cd->addOption(0, "I just remembered there's something I need to do, see you later!");
			ui->addTalkText(cd);
		}
		break;
	case 4: //Easter: Start tl;dr
		ui->addTalkText(new ChatInformation("Well, the basis of it is that my workers have gone through riot, and have messed up my magical bird that lays easter eggs.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("They have blocked off the bird's food storage, annoyed it, and messed up it's nest.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Now the bird is too stressed to lay eggs, and unless it does so there won't be easter this year, maybe any more!", chatNS::RIGHT));
		cd->addOption(5, "So, what do you need me to do?");
		cd->addOption(3, "Sounds like an interesting story. Could you tell me the full story?");
		cd->addOption(0, "I just remembered there's something I need to do, I need to go now.");
		ui->addTalkText(cd);
		break;
	case 5: //Easter: Start quest
		ui->addTalkText(new ChatInformation("I need you to get some feathers somwhere, in order to rebuild the bird's nest so that it is usuable again.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I also need you to get a sample of an easter egg that has already been sent out.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Finally, I require some cooked rotten meat to feed the bird, it hasn't been fed in a long time since the food supply has gone rotten when it was blocked", chatNS::RIGHT));
		gem->informListeners(new GameEvent_EntityAction(ii));
		cd->addOption(0, "Alright.");
		ui->addTalkText(cd);
		break;
	case 6: //Let's talk about something else
		ui->addTalkText(new ChatInformation("What would you like to know?.", chatNS::RIGHT));
		generateEasterQuestTalk(cd);
		ui->addTalkText(cd);
		break;
	case 7: //Let's talk about the feathers you need
		ui->addTalkText(new ChatInformation("Sure, what do you want to know?", chatNS::RIGHT));
		cd->addOption(8, "What do you need these feathers for?");
		if (questData->getValue("easterBirdNestStatus") != 2)
		{
			cd->addOption(9, "Where do you think I can get these feathers?");
		}
		else
		{
			cd->addOption(9, "I got some feathers, what should I do?");
		}
		cd->addOption(6, "Let's talk about something else");
		ui->addTalkText(cd);
		break;
	case 8: //What do you need these feathers for?
		ui->addTalkText(new ChatInformation("I need the feathers to rebuild the bird's nest to be like his home.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The bird can't lay eggs without having a proper, comfortable nest after all.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Thus, some feathers need to be gathered to place them at the nest to make it more comfortable.", chatNS::RIGHT));
		if (questData->getValue("easterBirdNestStatus") != 2)
		{
			cd->addOption(9, "Where do you think I can get these feathers?");
		}
		else
		{
			cd->addOption(9, "I got some feathers, what should I do?");
		}
		cd->addOption(6, "Let's talk about something else");
		ui->addTalkText(cd);
		break;
	case 9: //Where do you think I can get these feathers (if haven't gotten it yet)
		if (questData->getValue("easterBirdNestStatus") != 2)
		{
			ui->addTalkText(new ChatInformation("I have no idea where feathers can be easily found. Chickens, perhaps?", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("I know chickens can be found just near the doctor.", chatNS::RIGHT));
		}
		//I got some feathers, what should I do
		else
		{
			ui->addTalkText(new ChatInformation("Simply interact with the bird's nest to rebuild it.", chatNS::RIGHT));
		}
		cd->addOption(8, "What do you need these feathers for?");
		cd->addOption(6, "Let's talk about something else");
		ui->addTalkText(cd);
		break;
	case 10: //Let's talk about the easter egg you wanted.
		if (questData->getValue("easterEggStatus") == 2)
		{
			ui->addTalkText(new ChatInformation("I have an egg sample for you.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("You hand over the easter egg to the Easter Bunny.", chatNS::MIDDLE));
			gem->informListeners(new GameEvent_EntityAction(ii));
			ui->addTalkText(new ChatInformation("Thanks! This will help us identify which new eggs we can use!", chatNS::RIGHT));
			generateEasterQuestTalk(cd);
			ui->addTalkText(cd);
		}
		else
		{
			ui->addTalkText(new ChatInformation("Sure, what do you want to know?", chatNS::RIGHT));
			cd->addOption(11, "What do you need these eggs for?");
			cd->addOption(12, "Where do you think I can get the eggs?");
			cd->addOption(6, "Let's talk about something else");
			ui->addTalkText(cd);
		}
		break;
	case 11: //What do you need the easter egg for?
		ui->addTalkText(new ChatInformation("Well, not all eggs from the bird can be used as easter eggs.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Some eggs are of a bad quality, or may not contain what we need inside the egg.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("As such, it is best if we have a sample egg to compare new eggs to, so that we can ensure that we give out the good eggs instead of the bad.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Don't you have any eggs yourself?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Unfortunately not, during the riot all remaining eggs I have are destroyed. I will need you to get some back from people that I've given them to.", chatNS::RIGHT));
		cd->addOption(12, "Where do you think I can get the eggs?");
		cd->addOption(6, "Let's talk about something else");
		ui->addTalkText(cd);
		break;
	case 12: //Where do you think I can get the egg?
		ui->addTalkText(new ChatInformation("You might be able to get some back by killing monsters, they may already have gotten an easter egg from me, and will drop it if you kill them.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Alternatively, some people around town may already have them, but they won't be willing to part with them so easily - you may need to use other, less legal means to get them.", chatNS::RIGHT));
		cd->addOption(11, "What do you need these eggs for?");
		cd->addOption(6, "Let's talk about something else");
		ui->addTalkText(cd);
		break;
	case 13: //Let's talk about feeding the bird.
		ui->addTalkText(new ChatInformation("Sure, what do you want to know?", chatNS::RIGHT));
		cd->addOption(14, "What exactly do you need me to do?");
		cd->addOption(6, "Actually, let's talk about something else");
		ui->addTalkText(cd);
		break;
	case 14: //Feeding: What exactly do you need me to do?
		ui->addTalkText(new ChatInformation("I need you to get some cooked rotten meat and feed it to the bird right next to me.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Where can I get them?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("I think some of the stores around here may sell the meat, or you can kill some of the chickens and cook their meat", chatNS::RIGHT));
		generateEasterQuestTalk(cd);
		ui->addTalkText(cd);
		break;
	case 15: //About these easter eggs.
		ui->addTalkText(new ChatInformation("Ah yes. Thanks to you, I can now start giving out easter eggs to others!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("As such, during your travels, you may fight enemies that already have an easter eggs. They'll drop them on death.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Easter eggs come in 4 tiers - common, rare, epic and legendary. You can use these easter eggs for yourself or sell them to me.", chatNS::RIGHT));
		cd->addOption(16, "Tell me about the common easter eggs.");
		cd->addOption(17, "Tell me about the rare easter eggs.");
		cd->addOption(18, "Tell me about the epic easter eggs.");
		cd->addOption(19, "Tell me about the legendary easter eggs.");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
	case 16: //Tell me about the common easter eggs.
		ui->addTalkText(new ChatInformation("The common easter eggs are fairly common, and are likely the easter egg you will see the most.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("These easter eggs will restore 2HP once they are eaten. However, don't be fooled by their low rarity and low healing, they have a special property", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("And what would that be?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("They stack! They're common, stackable healing food! You can store pretty much limitless amount of healing in that single slot!", chatNS::RIGHT));
		cd->addOption(17, "Tell me about the rare easter eggs.");
		cd->addOption(18, "Tell me about the epic easter eggs.");
		cd->addOption(19, "Tell me about the legendary easter eggs.");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
	case 17: //Tell me about the rare easter eggs.
		ui->addTalkText(new ChatInformation("These easter eggs are significantly rarer and harder to find, however you can just buy one from me if you want one.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Their rarity is somewhat offset by their unique effect - they can be used to grant XP in a random skill!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("That's amazing!", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("I know! Additionally, if you get 4 common eggs in a row while killing monsters, the next egg you get is guaranteed to be a rare or better!", chatNS::RIGHT));
		cd->addOption(16, "Tell me about the common easter eggs.");
		cd->addOption(18, "Tell me about the epic easter eggs.");
		cd->addOption(19, "Tell me about the legendary easter eggs.");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
	case 18: //Tell me about the epic easter eggs.
		ui->addTalkText(new ChatInformation("The epic easter eggs are very rare and hard to find, but if your really want one you can purchase one from me.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("They will give you a teleport back to this town once used.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("So they can be used as an emergency escape?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("That's right! They work everywhere too!", chatNS::RIGHT));
		cd->addOption(16, "Tell me about the common easter eggs.");
		cd->addOption(17, "Tell me about the rare easter eggs.");
		cd->addOption(19, "Tell me about the legendary easter eggs.");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
	case 19: //Tell me about the legendary easter eggs.
		ui->addTalkText(new ChatInformation("Ah, the legendary easter eggs. Many never get one in their lifetime.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("However, they have amazing effects to make up for their rarity. For starters, they will restore all of your health! I won't spoil the remaining effects for you, you'll have to find them out yourself!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Ah, but if I really want one without having to leave my fate to RNGesus I can just buy one from you right?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("That is correct!", chatNS::RIGHT));
		cd->addOption(16, "Tell me about the common easter eggs.");
		cd->addOption(17, "Tell me about the rare easter eggs.");
		cd->addOption(18, "Tell me about the epic easter eggs.");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
		//===================== Shriveled man
	case 20: //Do you have a quest for me?
		ui->addTalkText(new ChatInformation("Well, uh... I don't think I", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The man pauses for a moment", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("I do have a quest for you, actually. I have an item, which was, well, you know, given to me by a precious friend. Unfortunately, it was stolen from me!", chatNS::RIGHT));
		cd->addOption(21, "Oh no! What can I do?");
		cd->addOption(22, "This looks like trouble... I don't think I want this quest.");
		ui->addTalkText(cd);
		break;
	case 21: //Start Stolen Artifact Quest (Oh no! What can I do? / All right then.)
		ui->addTalkText(new ChatInformation("The man who has stolen it lives some ways east of the doctor! Please, help me get the item back! It was rightfully mine after all!", chatNS::RIGHT));
		cd->addOption(0, "All right, I'll head to the house to scout out more");
		cd->addOption(23, "I'll help. I'm going to need more information though.");
		gem->informListeners(new GameEvent_EntityAction(ii));
		ui->addTalkText(cd);
		break;
	case 22: // This looks like trouble... I don't think I want this quest.
		ui->addTalkText(new ChatInformation("Oh, please help me! The uh, man is, well, you know, very arrogant and pompous, he just steals other people's artifacts for the sake of it!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Please help me get back one of my possessions!", chatNS::RIGHT));
		cd->addOption(0, "No thanks.");
		cd->addOption(21, "All right then.");
		ui->addTalkText(cd);
		break;
	case 23: //I'll help. I'm going to need more information though.
		ui->addTalkText(new ChatInformation("Well, you're going to need to get into the house to get the, um, stolen artifact back. To do so, you need to get into the house, either by using a key or picking the lock.", chatNS::RIGHT));
		cd->addOption(24, "How do I get the key?");
		cd->addOption(25, "How do I pick the lock?");
		cd->addOption(26, "You seem to hesitate a lot while speaking to me. What's up?");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
	case 24: //How do I get the key?
		ui->addTalkText(new ChatInformation("Thankfully, I have been, well, tracking the owner of the house for some time now. You understand, this is to ensure that I can easily provide help to an adeventurer like you who wishes to help me get into the house.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Of course, I understand", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Good. I've found that the man tends to look in his pond right before he enters his house... you may want to try looking there. Alternatively, the gardener outside his house probably has a key.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("You can also try talking to the gardener to find out any other places he may have hid a key.", chatNS::RIGHT));
		cd->addOption(25, "That all sounds complicated, how about picking the lock?");
		cd->addOption(26, "You seem to hesitate a lot while speaking to me. What's up?");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
	case 25: //How do I pick the lock?
		ui->addTalkText(new ChatInformation("Well, if you don't want to bother about getting the key, you can just pick the lock.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The lock looks pretty complicated though, so I'd expect you need a thieving level of at least 10 to pick it", chatNS::RIGHT));
		cd->addOption(24, "That's a rather high level, how about getting a key?");
		cd->addOption(26, "You seem to hesitate a lot while speaking to me. What's up?");
		cd->addOption(0, "Thanks for the information.");
		ui->addTalkText(cd);
		break;
	case 26: //You seem to hesitate a lot while speaking ot me. What's up?
		ui->addTalkText(new ChatInformation("Well, uh, you see, uh, well, basically, um, I tend to uh, be somewhat nervous.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Yeah! That's right! I tend to be nervous when, uh, asking people for help because I'd greatly prefer not to do that.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Well, if you say so...", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("This is pretty important to me though, so that's why I'm asking you for help.", chatNS::RIGHT));
		cd->addOption(24, "How do I get the key?");
		cd->addOption(25, "How do I pick the lock?");
		cd->addOption(0, "I'd better get to doing it then.");
		ui->addTalkText(cd);
		break;
		// ====================== Gardener
	case 27: // What do you do here?
		ui->addTalkText(new ChatInformation("Oh, I'm the gardener of this house. As you can see, it has a pretty big garden, even with a pond.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("So I'm helping to manage it. You know, grow crops, plant flowers, make it nicer, the like.", chatNS::RIGHT));
		cd->addOption(28, "But there doesn't seem to be any flowers in the garden...");
		cd->addOption(29, "So you're the gardener? Can you enter this house?");
		ui->addTalkText(cd);
		break;
	case 28: //But there doesn't seem to be many cops around here.
		ui->addTalkText(new ChatInformation("How dare you insult me like that? I'll have you know that I'm a master gardener and can grow crops in the most barren of land!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The gardener looks around him", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("All right, I'll admit, this land looks slightly empty at the moment. Give it time. Soon you'll come here just to experience its beauty!", chatNS::RIGHT));
		cd->addOption(29, "Are you allowed to enter the house?");
		cd->addOption(0, "I'll leave you to it then.");
		ui->addTalkText(cd);
		break;
	case 29: //Can you enter the house?
		if (questData->getValue("artifactStealKey") == 0)
		{
			ui->addTalkText(new ChatInformation("Of course I can! I have the key right here, with me.", chatNS::RIGHT));
			if (questData->getValue("artifactStealStatus") == 1)
			{
				cd->addOption(30, "Awesome. Can I borrow it?");
			}
			cd->addOption(0, "Alright, thanks");
			ui->addTalkText(cd);
		}
		else
		{
			ui->addTalkText(new ChatInformation("Of course I can! I loaned you the key, remember?", chatNS::RIGHT));
			cd->addOption(0, "Oh yeah...");
			ui->addTalkText(cd);
		}
		break;
	case 30: //Can I borrow the key?
		ui->addTalkText(new ChatInformation("Well, I'm actually not allowed to. But since I'm a nice guy, how about this? If you beat me in a challenge, I'll let you borrow the key.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I'll need you to drink finish the water in here. If you can do that, I'll lend you the key.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("He shows you the water. It's all murky, you can't tell what's inside it.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("You estimate you need a defense level of at least 6 and 14 Health to stomach that.", chatNS::MIDDLE));
		if (thePlayer->getSkills()->at(skillNS::ID_SKILL_DEFENSE).getSkillLevel() >= 6 && thePlayer->getHealth() >= 14)
		{
			cd->addOption(31, "All right, I'll drink it.");
		}
		cd->addOption(32, "What's inside that thing?");
		cd->addOption(0, "Ew, no thanks.");
		ui->addTalkText(cd);
		break;
	case 31: //All right, I'll drink it (only selectable with >= 6 defense and >=14 HP
		ui->addTalkText(new ChatInformation("Steeling yourself, you take the drink and finish it", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("The drink mixes in your stomach...you feel a large pain inside", chatNS::MIDDLE));
		thePlayer->damage(13);
		ui->addTalkText(new ChatInformation("Nicely done. Here, have the key.", chatNS::RIGHT));
		gem->informListeners(new GameEvent_ItemReceived(grpg->getItemLoader()->getItem(36)));
		questData->setValue("artifactStealMethod", 4);
		cd->addOption(0, "Thanks.");
		ui->addTalkText(cd);
		break;
	case 32: //What's in the drink?
		ui->addTalkText(new ChatInformation("Why, that's for me to know and for you to find out...by drinking it!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("So, want to try it?", chatNS::RIGHT));
		if (thePlayer->getSkills()->at(skillNS::ID_SKILL_DEFENSE).getSkillLevel() >= 6 && thePlayer->getHealth() >= 14)
		{
			cd->addOption(31, "All right, I'll drink it.");
		}
		cd->addOption(0, "Ew, no thanks.");
		ui->addTalkText(cd);
		break;
	case 33: // ============= Shriveled man ============== Um, are you ok?
		ui->addTalkText(new ChatInformation("Why, yes, I'm ok. Especially ok now that you've helped me with bringing back this artifact that I will keep!", chatNS::RIGHT));
	case 34: // How about a reward?
		ui->addTalkText(new ChatInformation("Oh yes, your reward... here you go, as payment for getting back this artifact of Do- ... I mean just this artifact.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Now, I'll be going now, if you don't mind.", chatNS::RIGHT));
		gem->informListeners(new GameEvent_EntityAction(ii));
		cd->addOption(35, "Uh, sure");
		ui->addTalkText(cd);
		break;
	case 35: //Start the stolen artifact run from the shriveled man
		ui->removeWindow();
		//No use with cd
		delete cd;
		((ShriveledMan*)entity)->startStolenArtifactRun();
		break;
	case 36: // ============ Alfred ================= Who are you?
		ui->addTalkText(new ChatInformation("I'm Alfred. I'm the gardener for this area here. I keep the place tidy, plant flowers, and basically make it look nice.", chatNS::RIGHT));
		cd->addOption(37, "What happened to the previous guy?");
		cd->addOption(38, "No offense, but there doesn't seem to be much of a garden here.");
		if (questData->getValue("mysteriousArtifactStatus") == 0)
		{
			cd->addOption(39, "Do you need my help?");
		}
		else
		{
			cd->addOption(44, "Tell me about the tasks that needed doing.");
			if (questData->getValue("mysteriousArtifactWaitTime") == 1)
			{
				cd->addOption(53, "I managed to plant the flower seeds, now what?");
			}
			else if (questData->getValue("mysteriousArtifactWaitTime") == 2)
			{
				cd->addOption(53, "I managed to plant the tree seed into the pot, now what?");
			}
			else if (questData->getValue("mysteriousArtifactWaitTime") == 3)
			{
				cd->addOption(53, "I managed to transfer the young tree into the patch, now what?");
			}
		}
		ui->addTalkText(cd);
		break;
	case 37: //What happened to the previous guy?
		ui->addTalkText(new ChatInformation("Oh, him? I'm not too sure of the details of the person before me, but I've heard rumours...", chatNS::RIGHT));
		if (questData->getValue("artifactStealMethod") == 0) //Unknown? This should never occur cause the player should have did something to get the key to complete the quest & get alfred to spawn, but just in case...
		{
			ui->addTalkText(new ChatInformation("Unforunately they are very vague, and some even contradict each other, no one really knows what happened.", chatNS::RIGHT));
		}
		else if (questData->getValue("artifactStealMethod") == 1) //Player fished for the key
		{
			ui->addTalkText(new ChatInformation("The most often heard rumour is that he wasn't doing his job properly, flowers wasn't being planted and the garden was still looking really bad, so he got fired for not doing his job.", chatNS::RIGHT));
		}
		else if (questData->getValue("artifactStealMethod") == 2) //Player picked the lock for access
		{
			ui->addTalkText(new ChatInformation("The most often heard rumour is that he didn't properly guard the door, but allowed a thief to simply come in and pick the lock.", chatNS::RIGHT));
		}
		else if (questData->getValue("artifactStealMethod") == 3) //Player killed the guy
		{
			ui->addTalkText(new ChatInformation("The most often heard rumour is that he got killed by a wandering adventurer! Who knew being a gardener could get you killed like that!", chatNS::RIGHT));
		}
		else if (questData->getValue("artifactStealMethod") == 4) //Player took toughness test
		{
			ui->addTalkText(new ChatInformation("The most often heard rumour is that he gave away the key to the house, which of course cannot be tolerated as a gardener, and was thus fired.", chatNS::RIGHT));
		}
		cd->addOption(36, "Who are you?");
		if (questData->getValue("mysteriousArtifactStatus") == 0)
		{
			cd->addOption(39, "Do you need my help with anything?");
		}
		else
		{
			cd->addOption(44, "Tell me about the tasks that needed doing.");
			if (questData->getValue("mysteriousArtifactWaitTime") == 1)
			{
				cd->addOption(53, "I managed to plant the flower seeds, now what?");
			}
			else if (questData->getValue("mysteriousArtifactWaitTime") == 2)
			{
				cd->addOption(53, "I managed to plant the tree seed into the pot, now what?");
			}
			else if (questData->getValue("mysteriousArtifactWaitTime") == 3)
			{
				cd->addOption(53, "I managed to transfer the young tree into the patch, now what?");
			}
		}
		ui->addTalkText(cd);
		break;
	case 38: //No offense, but there doesn't seem to be much of a garden here.
		ui->addTalkText(new ChatInformation("None taken. I understand what you mean. I think the last gardener was far too boastful of himself, refusing any help. That's why the garden is in this state now.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("However, I'm hoping to change that, and make the gardener look better. It's likely that I'm going to need some help though.", chatNS::RIGHT));
		cd->addOption(37, "What happened to the last gardener?");
		if (questData->getValue("mysteriousArtifactStatus") == 0)
		{
			cd->addOption(39, "What do you need my help with?");
		}
		else
		{
			cd->addOption(44, "Tell me about the tasks that needed doing.");
			if (questData->getValue("mysteriousArtifactWaitTime") == 1)
			{
				cd->addOption(53, "I managed to plant the flower seeds, now what?");
			}
			else if (questData->getValue("mysteriousArtifactWaitTime") == 2)
			{
				cd->addOption(53, "I managed to plant the tree seed into the pot, now what?");
			}
			else if (questData->getValue("mysteriousArtifactWaitTime") == 3)
			{
				cd->addOption(53, "I managed to transfer the young tree into the patch, now what?");
			}
		}
		cd->addOption(0, "I can't right now, sorry.");
		ui->addTalkText(cd);
		break;
	case 39:
		ui->addTalkText(new ChatInformation("Yes, I do actually.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("There's been a bit of trouble around here these days, an artifact belonging to my employer apparently got stolen by some hooligan.", chatNS::RIGHT));
		if (questData->getValue("artifactStealMethod") == 2) //Lock was picked
		{
			ui->addTalkText(new ChatInformation("The lock on the door was picked and broken, allowing anyone to just enter the house.", chatNS::RIGHT));
		}
		else if (questData->getValue("artifactStealMethod") == 3) //Gardener was killed
		{
			ui->addTalkText(new ChatInformation("The previous gardener was killed in order to get his key to perform the theft!", chatNS::RIGHT));
		}
		else
		{
			ui->addTalkText(new ChatInformation("The thief managed to get a key to the house somehow, allowing him to enter the house", chatNS::RIGHT));
		}
		ui->addTalkText(new ChatInformation("Oh, well, um...", chatNS::LEFT));
		cd->addOption(40, "Actually, I stole the artifact.");
		cd->addOption(41, "That's bad!");
		ui->addTalkText(cd);
		break;
	case 40: //Actually, I stole the artifact
		questData->setValue("artifactStealAdmitted", 1);
		ui->addTalkText(new ChatInformation("You? Stole the artifact?", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Don't make me laugh. You don't seem capable of fighting off a goblin, let alone breaking into a house and stealing the artifact!", chatNS::RIGHT));
		cd->addOption(42, "So, how can I help?");
		ui->addTalkText(cd);
		break;
	case 41: //That's bad!
		questData->setValue("artifactStealAdmitted", 0);
		ui->addTalkText(new ChatInformation("Yup. This area may not be as safe as it would originally seem.", chatNS::RIGHT));
		cd->addOption(42, "So, how can I help?");
		ui->addTalkText(cd);
		break;
	case 42: //Continuation after artifact talk.
		ui->addTalkText(new ChatInformation("The theft has made people afraid to work with me these days, so I'm asking you for help.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I'd like to make this garden look nicer, plant some flowers, grow a tree, and bring some fishes into the pond", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Do you think you can help me wih that?", chatNS::RIGHT));
		cd->addOption(43, "Of course I can, count me in!");
		cd->addOption(44, "Tell me what needs to be done first.");
		cd->addOption(0, "I'm afraid I'm not much of a gardener.");
		ui->addTalkText(cd);
		break;
	case 43: //Of course I can, count me in! (Start Quest)
		ui->addTalkText(new ChatInformation("Awesome! Thank you!", chatNS::RIGHT));
		gem->informListeners(new GameEvent_EntityAction(ii));
	case 44: //Tell me what needs to be done first (During or before quest, to find out details)
		ui->addTalkText(new ChatInformation("There are 3 tasks I need done, however, for simplicity I recommend just doing them in the order I recommend.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The tasks are: planting flowers, growing a tree and adding fish to the pond. I recommend starting with planting flowers first.", chatNS::RIGHT));
		cd->addOption(45, "Tell me about planting flowers.");
		cd->addOption(46, "Tell me about growing trees");
		cd->addOption(47, "Tell me about adding fish.");
		if (questData->getValue("mysteriousArtifactOtherTask") == 0)
		{
			cd->addOption(48, "I don't want to do them in order, can I choose not to?");
		}
		if (questData->getValue("mysteriousArtifactStatus") == 0) //If quest not yet started
		{
			cd->addOption(43, "I've heard enough. I've decided to help you.");
		}
		cd->addOption(0, "All right, thanks.");
		ui->addTalkText(cd);
		break;
	case 45: //Tell me about planting flowers.
		ui->addTalkText(new ChatInformation("I need you to plant some flowers to make the garden look nicer.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("This task should be the easiest for you and I recommend that you start with it, as the gardener I can provide you with the supplies required to do them", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("All you have to do is plant the seeds and wait for them to grow.", chatNS::RIGHT));
		cd->addOption(44, "Tell me about the tasks again.");
		cd->addOption(46, "Tell me about growing trees");
		cd->addOption(47, "Tell me about adding fish.");
		if (questData->getValue("mysteriousArtifactOtherTask") == 0)
		{
			cd->addOption(48, "I don't want to do them in order, can I choose not to?");
		}
		if (questData->getValue("mysteriousArtifactStatus") == 0) //If quest not yet started
		{
			cd->addOption(43, "I've heard enough. I've decided to help you.");
		}
		//Quest started and no task chosen yet.
		else if (questData->getValue("mysteriousArtifactStatus") == 1 && questData->getValue("mysteriousArtifactGardenerTask") == 0)
		{
			cd->addOption(49, "Let's start with this task.");
		}
		cd->addOption(0, "All right, thanks.");
		ui->addTalkText(cd);
		break;
	case 46: //Tell me about growing trees
		ui->addTalkText(new ChatInformation("I need you to plant a tree in this garden, it looks bland if it's just flowers.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I recommend doing this task as your second tasks. Trees need to be first nurtured in a plant pot and grown first, then transferred to the soil.", chatNS::RIGHT));
		cd->addOption(44, "Tell me about the tasks again.");
		cd->addOption(45, "Tell me about planting flowers.");
		cd->addOption(47, "Tell me about adding fish.");
		if (questData->getValue("mysteriousArtifactOtherTask") == 0)
		{
			cd->addOption(48, "I don't want to do them in order, can I choose not to?");
		}
		if (questData->getValue("mysteriousArtifactStatus") == 0) //If quest not yet started
		{
			cd->addOption(43, "I've heard enough. I've decided to help you.");
		}
		//if quest started, the player has requested to pick a task other than the recommend task, and no task has been picked yet.
		else if (questData->getValue("mysteriousArtifactStatus") == 1 && questData->getValue("mysteriousArtifactOtherTask") == 1 && questData->getValue("mysteriousArtifactGardenerTask") == 0)
		{
			cd->addOption(50, "Let's start with this task.");
		}
		cd->addOption(0, "All right, thanks.");
		ui->addTalkText(cd);
		break;
	case 47: //Tell me about adding fish
		ui->addTalkText(new ChatInformation("I need you to populate the pond with fishes. Addiionally, I would like to be able to ensure that the fishes can survive and reproduce, so that they can slowly populate the pond and do well.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("This task will likely be the hardest; I recommend doing it last. You'll need some way to transfer live fish over to here, without it dying.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Nothing springs to mind on how to complete it at the moment.", chatNS::RIGHT));
		cd->addOption(44, "Tell me about the tasks again.");
		cd->addOption(45, "Tell me about planting flowers.");
		cd->addOption(46, "Tell me about growing trees");
		if (questData->getValue("mysteriousArtifactOtherTask") == 0)
		{
			cd->addOption(48, "I don't want to do them in order, can I choose not to?");
		}
		if (questData->getValue("mysteriousArtifactStatus") == 0) //If quest not yet started
		{
			cd->addOption(43, "I've heard enough. I've decided to help you.");
		}
		else if (questData->getValue("mysteriousArtifactStatus") == 1 && questData->getValue("mysteriousArtifactOtherTask") == 1 && questData->getValue("mysteriousArtifactGardenerTask") == 0)
		{
			cd->addOption(51, "Let's start with this task.");
		}
		cd->addOption(0, "All right, thanks.");
		ui->addTalkText(cd);
		break;
	case 48: //I don't want to do this in order, can I choose not to?
		questData->setValue("mysteriousArtifactOtherTask", 1);
		ui->addTalkText(new ChatInformation("Well, sure, I suppose you can.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I wouldn't recommend it though.", chatNS::RIGHT));
		cd->addOption(44, "Tell me about the tasks again.");
		cd->addOption(45, "Tell me about planting flowers.");
		cd->addOption(46, "Tell me about growing trees");
		cd->addOption(47, "Tell me about adding fish.");
		if (questData->getValue("mysteriousArtifactStatus") == 0) //If quest not yet started
		{
			cd->addOption(43, "I've heard enough. I've decided to help you.");
		}
		cd->addOption(0, "All right, thanks.");
		ui->addTalkText(cd);
		break;
	case 49: //Let's start with this task (Planting flowers)
		questData->setValue("mysteriousArtifactGardenerTask", 1);
		ui->addTalkText(new ChatInformation("Awesome! You'll need the following items, of course.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Alfred hands you a rake, a spade, a seed dibbler and some flower seeds.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("I am going to need you to plant these in the flower patch. You'll need to rake away the weeds currently covering the patch, and then dig a hole and plant the seeds in them.", chatNS::RIGHT));
		cd->addOption(0, "All right, thanks.");
		gem->informListeners(new GameEvent_EntityAction(ii));
		ui->addTalkText(cd);
		break;
	case 50: //Let's start with this task (Planting trees)
		{
			questData->setValue("mysteriousArtifactGardenerTask", 2);
			ui->addTalkText(new ChatInformation("Well, I wouldn't really recommend it, like I said.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("But if you want to, you can. Here's a the items that you'll need.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("Alfred hands you a tree seed, a plant pot and a watering can.", chatNS::MIDDLE));
			//Give the player a plant pot. He actually needs to interact with it
			Entity* plantpot = new Entity();
			plantpot->initialize(grpg, new InventoryItem(grpg->getItemLoader()->getItem(38), 1));
			grpg->getPlayer()->getInventory()->addEntityInventoryItem(plantpot, grpg);
			ui->addTalkText(new ChatInformation("You'll need to plant the seed into the pot, water it and wait for it to grow, before transferring it to the soil.", chatNS::RIGHT));
			cd->addOption(0, "All right, thanks");
			gem->informListeners(new GameEvent_EntityAction(ii));
			ui->addTalkText(cd);
			break;
		}
	case 51: //Let's start with this task (Adding fish)
		ui->addTalkText(new ChatInformation("Well, I wouldn't really recommend it, like I said earlier, especially as I have no idea how to get this done for now.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("If you give me some time, and do some of the other tasks first, I might think of sometihing for this.", chatNS::RIGHT));
		cd->addOption(52, "I'd like to start with this first anyway.");
		cd->addOption(44, "Very well, what were the other tasks?");
		ui->addTalkText(cd);
		break;
	case 52: //I'd like to start with this first anyway (adding fish)
		questData->setValue("mysteriousArtifactGardenerTask", 3);
		ui->addTalkText(new ChatInformation("If you say so. You're on you're own, however, I have no good ideas to do this. Maybe you can think of one?", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Remember, you'll need to get some live fishes, and find some way to transfer them to the pond, without them dying.", chatNS::RIGHT));
		cd->addOption(0, "I'll get right on it.");
		gem->informListeners(new GameEvent_EntityAction(ii));
		//Spawn the rift afterwards and move on the quest.
		questData->setValue("mysteriousArtifactStatus", 2);
		ui->addTalkText(cd);
		break;
	case 53: //I managed to plant the flowers/tree seed/young tree, now what?
		ui->addTalkText(new ChatInformation("Awesome. Now, all you have to do is to wait for the plant to grow.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("However, if you want, I can magically grow the plant immediately, without needing you to wait!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("You'll have to pay me $2 in USD for that though", chatNS::RIGHT));
		cd->addOption(55, "I'm not paying you that money!");
		cd->addOption(54, "Will this cause all plants I grow later to instantly grow?");
		cd->addOption(55, "That price is absurd!");
		cd->addOption(56, "Why should I pay you that money when I'm helping you?");
		ui->addTalkText(cd);
		break;
	case 54: //Will this cause all plants I grow in the future to instantly grow as well?
		ui->addTalkText(new ChatInformation("No, of course not. For every plant you grow afterwards that you want to grow immediately, you must pay me USD $2 to grow it.", chatNS::RIGHT));
		cd->addOption(55, "I'm not paying you that money!");
		cd->addOption(54, "Well this cause all plants I grow later to instantly grow?");
		cd->addOption(55, "That price is absurd!");
		cd->addOption(56, "Why should I pay you that money when I'm helping you?");
		ui->addTalkText(cd);
		break;
	case 55: //I'm not paying you that money/that price is absurd!
		questData->setValue("mysteriousArtifactStatus", 2);
		ui->addTalkText(new ChatInformation("Well, if you're not paying me that money I guess you'll just have to wait for the plant to grow then.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Go off and adventure a bit, I suppose, it'll grow eventually.", chatNS::RIGHT));
		cd->addOption(0, "Oh, I'll do that.");
		cd->addOption(57, "How about I do your other tasks while waiting?");
		ui->addTalkText(cd);
		break;
	case 56: //Why should I pay you that money when I'm helping you grow it for you?
		ui->addTalkText(new ChatInformation("Because you want to complete the quest quicker, of course.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Here's a thought for you - why do people play a farming simulation game, and then pay to have their plants grow quickly instead of waiting for them to grow?", chatNS::RIGHT));
		cd->addOption(55, "I'm not paying you that money!");
		cd->addOption(54, "Well this cause all plants I grow later to instantly grow?");
		cd->addOption(55, "That price is absurd!");
		ui->addTalkText(cd);
		break;
	case 57: //How about I do your other tasks while waiting?
		ui->addTalkText(new ChatInformation("Patience. One task at a time, let's not rush ourselves here. Go do some other adventures for a while, I'm sure it'll grow by then.", chatNS::RIGHT));
		cd->addOption(0, "Ok");
		ui->addTalkText(cd);
		break;
		 //================ Shriveled man part 2: Mysterious Artifact ======
	case 58: //What is this place?
		questData->setValue("darkRealmName", 1);
		ui->addTalkText(new ChatInformation("You like it? They call this the Dark Realm.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("You can kind of see why they do call it the dark realm, it's so much darker in here after all.", chatNS::RIGHT));
		cd->addOption(59, "What's going on here?");
		cd->addOption(60, "What have you done?");
		ui->addTalkText(cd);
		break;
	case 59: //What's going on here?
		ui->addTalkText(new ChatInformation("Ah, the young and naive hero. Since you seem to know nothing, let me educate you.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I have used the artifact - that you have so kindly stole for me, by the way - and, shall we say, helped the denizens of this realm.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Isn't that what we're all for, anyone? Helping others? Here, I'm helping them to realize their full potential!", chatNS::RIGHT));
		cd->addOption(58, "What is this place?");
		cd->addOption(60, "What have you done?");
		ui->addTalkText(cd);
		break;
	case 60: //What have you done?
		ui->addTalkText(new ChatInformation("Nothing much, really. I've managed to simply get them to congregate in one location, funneling their power.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Oh, here they are, right on cue. I hope you're prepared, hero!", chatNS::RIGHT));
		cd->addOption(61, "What are you talking about?");
		ui->addTalkText(cd);
		break;
	case 61: //What are you talking about?
		((ShriveledMan*)entity)->continueRiftIntro();
		gem->informListeners(new GameEvent_EntityAction(ii));
		ui->removeWindow();
		break;
		// ========================== Vangel ========= (End of Mysterious Artifact Quest)
	case 62: //Actually... I stole the artifact
		questData->setValue("artifactStealAdmitted", 1);
		ui->addTalkText(new ChatInformation("I know.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("You did? But... how?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Do you not think I have cameras in my own house to catch people who are attempting to steal the artifact? You were simply lucky that I was out of town that day, and thus couldn't stop you.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("At least you're honest about it.", chatNS::RIGHT));
		cd->addOption(63, "What was that place?");
		cd->addOption(64, "How did that portal appear?");
		cd->addOption(65, "So, what now?");
		ui->addTalkText(cd);
		break;
	case 63: //What was that place?
		if (questData->getValue("darkRealmName") == 0)
		{
			ui->addTalkText(new ChatInformation("People call it the Dark Realm, likely due to how everything's darker there.", chatNS::RIGHT));
		}
		else
		{
			ui->addTalkText(new ChatInformation("The shriveled man in there called it the Dark Realm.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("That's right.", chatNS::RIGHT));
		}
		ui->addTalkText(new ChatInformation("It holds plenty of monsters, many who would be more than happy to invade our world in large amounts, instead of just the few camps we have here and there.", chatNS::RIGHT));
		cd->addOption(64, "How did that portal appear?");
		if (questData->getValue("mysteriousArtifactStatus") == 7)
		{
			cd->addOption(65, "So, what now?");
		}
		else
		{
			cd->addOption(69, "What happens when I enter a rift?");
			cd->addOption(70, "What should I expect when I enter a rift?");
			cd->addOption(0, "I have no more questions.");
		}
		ui->addTalkText(cd);
		break;
	case 64: //How did that portal appear? 
		ui->addTalkText(new ChatInformation("The monsters in there have one power - they are able to create a rift to this world, by gathering together and forcusing their power into a single area.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("However, they need a catalyst to magnify their power - and this is where the artifact comes in.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Essentially, the artifact provides the means for the holder to help the monsters in the Dark Realm focus their power, and create more rifts into this world, which can potentially allow the monsters to break through to here.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Hmm, the shriveled man did say something like that", chatNS::LEFT));
		cd->addOption(63, "What was that place?");
		if (questData->getValue("mysteriousArtifactStatus") == 7)
		{
			cd->addOption(65, "So, what now?.");
		}
		else
		{
			cd->addOption(69, "What happens when I enter a rift?");
			cd->addOption(70, "What should I expect when I enter a rift?");
			cd->addOption(0, "I have no more questions.");
		}
		ui->addTalkText(cd);
		break;
	case 65: //I have no questions/So, what now?
		ui->addTalkText(new ChatInformation("I have sent word to my scouts to try and track down that man and get the artifact back", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I will alert you once I have learned anything.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("In the meantime, if you find any more of those rifts, please enter them and help to seal them - leaving them open for far too long will allow the monsters to pour into our whold!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Here's a sword - make good use of it", chatNS::RIGHT));
		if (questData->getValue("mysteriousArtifactGardenerTask") == 1)
		{
			ui->addTalkText(new ChatInformation("Oh, and Alfred says he wants to talk to you about those flowers he asked you to grow.", chatNS::RIGHT));
		}
		else if (questData->getValue("mysteriousArtifactGardenerTask") == 2)
		{
			ui->addTalkText(new ChatInformation("Oh, and you might want to talk to Alfred about that plant pot.", chatNS::RIGHT));
		}
		cd->addOption(66, "Will do.");
		ui->addTalkText(cd);
		break;
	case 66: //Will do
		{
			gem->informListeners(new GameEvent_EntityAction(ii));
			ui->removeWindow();
			//No use with cd
			delete cd;
			grpg->attemptQuestCompletions();
			//Also move the people back to their positions
			//Move alfred back to starting point
			//Get alfred
			stringstream ss;
			VECTOR2 location = grpg->getMapLoader()->translateIdToCoords('\\');
			ss << location.x << "," << location.y;
			Entity* alfred = grpg->getSpawnLink(ss.str());
			alfred->setSpawnPoint(location);
			alfred->setDestination(new Point(location));
			//Move Vangel to the tile right below the artifact display case
			VECTOR2 location2 = grpg->getMapLoader()->translateIdToCoords('#');
			location2.y += tileNS::HEIGHT;
			entity->setDestination(new Point(location2));/*
			//Spawn 10 rifts at random locations
			VECTOR2 topLeftAllowedSpawn = grpg->getMapLoader()->translateIdToCoords('{');
			VECTOR2 bottomRightAllowedSpawn = grpg->getMapLoader()->translateIdToCoords('}');
			for (int i = 0; i < 10; i++)
			{
				//Pick a random location
				float randomX = rand() * 1.0 / RAND_MAX;
				randomX = randomX * (bottomRightAllowedSpawn.x - topLeftAllowedSpawn.x);
				randomX += topLeftAllowedSpawn.x;
				float randomY = rand() * 1.0 / RAND_MAX;
				randomY = randomY * (bottomRightAllowedSpawn.y - topLeftAllowedSpawn.y);
				randomY += topLeftAllowedSpawn.y;
				VECTOR2 newLocation = VECTOR2(randomX, randomY);
				NPC::spawn(grpg, 39, newLocation);
			}*/
			break;
		}
	case 67: //Alfred again - but what about your other tasks?
		ui->addTalkText(new ChatInformation("Hey, don't worry about those.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("You have more important stuff to deal with, like closing those rifts and preventing the Dark Realm from breaking into this world.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I'll improve the garden myself - it's my job after all!", chatNS::RIGHT));
		cd->addOption(68, "What do you know about the artifact?");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 68: //What do you know about the artifact?
		ui->addTalkText(new ChatInformation("Nothing much, I'm afraid. If you want to know most of the information, you'll have to talk with the actual owner.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("What I do know is that it holds a lot of power in the Dark Realm, and can cause rifts between our world and the Dark Realm.", chatNS::RIGHT));
		cd->addOption(67, "What about the other tasks?");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 69: //Vangel =========================== What happens when I enter a rift
		ui->addTalkText(new ChatInformation("Once you'll enter a rift, you'll go into the Dark Realm.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The Dark Realm's structure is mostly the same as our world, so you'll find that many structures that exist in our world will do so in the Dark Realm.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Obviously, people and items here will not appear in the dark realm, and vice versa.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I strongly advise that you do not go too far away from the rift, lest it close and you end up trapped inside the dark realm.", chatNS::RIGHT));
		cd->addOption(63, "What was that place?");
		cd->addOption(64, "How did the portal appear?");
		cd->addOption(70, "What should I expect when I enter a rift?");
		cd->addOption(0, "I have no more questions.");
		ui->addTalkText(cd);
		break;
	case 70: //What should I expect when I enter a rift?
		ui->addTalkText(new ChatInformation("Plenty. My scouts have entered rifts themselves and report that they all follow a similar structure.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("In essence, you can enter a rift to fight waves of monsters that will come. Once you have completed the rift, you can then leave it for some rift sealing xp. Fully completing the rift will also grant you a large chunk of bonus xp.", chatNS::RIGHT));
		cd->addOption(71, "Tell me about the waves in a rift.");
		cd->addOption(72, "What happens if I want to exit the rift early?");
		cd->addOption(73, "Tell me about the experience rewards.");
		cd->addOption(74, "Let's focus less on the rift.");
		ui->addTalkText(cd);
	break;
	case 71: //Tell me about the waves.
		ui->addTalkText(new ChatInformation("Once you enter the rift, you'll see some monsters nearby that caused the rift to open.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Once you kill these, harder and harder monsters will come to back them up.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("After the first batch of monsters, you'll find that rifts can have 2-5 waves more of enemies.", chatNS::RIGHT));
		cd->addOption(72, "What happens if I want to exit the rift early?");
		cd->addOption(73, "Tell me about the experience rewards.");
		cd->addOption(74, "Let's focus less on the rift.");
		ui->addTalkText(cd);
		break;
	case 72: //What happens if I want to exit the rift early?
		ui->addTalkText(new ChatInformation("If you need to exit the rift, you can choose to exit it anytime by clicking on the portal.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The best time to exit is when reinforcements are still arriving and no one is attcking you.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("However, if need be you can choose to make a hasty escape while monsters are still attacking you, but you will take some extra damage as monsters hammer at you while you leave.", chatNS::RIGHT));
		cd->addOption(71, "Tell me about the waves in a rift.");
		cd->addOption(73, "Tell me about the experience rewards.");
		cd->addOption(74, "Let's focus less on the rift.");
		ui->addTalkText(cd);
		break;
	case 73: //Tell me about the experience rewards.
		ui->addTalkText(new ChatInformation("As you close rifts you will gain experience in rift-sealing.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("If you manage to fully close the rift, clearing all the waves, you will gain a chunk of experience as a bonus.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("If you leave early, you will still gain experience based on the waves that you have cleared, but only half of the experience you would have gained had you completed the rift.", chatNS::RIGHT));
		cd->addOption(71, "Tell me about the waves in a rift.");
		cd->addOption(72, "What happens if I want to exit the rift early?");
		cd->addOption(74, "Let's focus less on the rift.");
		ui->addTalkText(cd);
		break;
	case 74: //Let's focus less on the rift (Return to previous conversation menu)
		ui->addTalkText(new ChatInformation("What would you like to know?", chatNS::RIGHT));
		cd->addOption(63, "What was that place?");
		cd->addOption(64, "How did the portal appear?");
		cd->addOption(69, "What happens when I enter a rift?");
		cd->addOption(70, "What should I expect when I enter a rift?");
		cd->addOption(0, "I have no more questions.");
		ui->addTalkText(cd);
		break;
	case 75: //How do I fish?
		ui->addTalkText(new ChatInformation("Fishing is easy! Simply walk up to a fishing spot, like the ones right beside me.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Once there, click on a fishing spot to immediately begin fishing. You will fish automatically, over time.", chatNS::RIGHT));
		cd->addOption(76, "How do I get better at fishing?");
		cd->addOption(77, "What can I get from fishing?");
		cd->addOption(0, "That's all, thanks.");
		ui->addTalkText(cd);
		break;
	case 76: //How do I get better at fishing?
		ui->addTalkText(new ChatInformation("Simple. To get better at fishing, you need to fish more!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("As you catch fish, you'll naturally grow more proficient at fishing.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("As your fishing level improves, you'll also start to catch fish faster, which means that you'll gain more experience! It's a great cycle!", chatNS::RIGHT));
		cd->addOption(75, "How do I fish?");
		cd->addOption(77, "What can I get from fishing?");
		cd->addOption(0, "That's all, thanks.");
		ui->addTalkText(cd);
		break;
	case 77: //What can I get from fishing??
		ui->addTalkText(new ChatInformation("Fish, of course, what else?", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Why would I want fishes?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Why would you not want fishes? You can sell them to others for gold if you want.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Alternatively, you can cook them with your cooking skill and then eat them to restore your health.", chatNS::RIGHT));
		cd->addOption(75, "How do I fish?");
		cd->addOption(76, "How do I get better at fishing?");
		cd->addOption(78, "Will you buy fish from me?");
		cd->addOption(0, "That's all, thanks.");
		ui->addTalkText(cd);
		break;
	case 78: //Will you buy fish from me?
		ui->addTalkText(new ChatInformation("Me? No, why would I do that? I can catch all the fish I want!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Not everyone is as skilled as me, though. Try the Grocer or the Blacksmith - they might be willing to buy some fish.", chatNS::RIGHT));
		cd->addOption(75, "How do I fish?");
		cd->addOption(76, "How do I get better at fishing?");
		cd->addOption(0, "That's all, thanks.");
		ui->addTalkText(cd);
		break;
	case 79: //I'm ready, bring me to the island!
		ui->addTalkText(new ChatInformation("Of course. For future reference, if you want me to bring you to the island quickly you can simply right click on me and choose to request a teleport.", chatNS::RIGHT));
		cd->addOption(0, "Thanks for the trip!");
		cd->addOption(0, "I'll keep that in mind.");
		ui->addTalkText(cd);
		entity->teleportBehavior->action();
		break;
	case 80: //Tell me about the great and powerful dragon.
		if (questData->getValue("minorTaskStatus") == 5) //Complete!
		{
			ui->addTalkText(new ChatInformation("Sure. I have never fought the dragon, but I've heard stories from people fighting it.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("I'd just like to warn you that these stories may not be entirely accurate; but may instead contain some false or exaggerated information. They are stories, after all.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("The tales say that Aildiuln has 3 phrases in his fights, as well as the ability to breathe magical dragonfire.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("As Aildiuln's health depletes, he will change to a different phrase.", chatNS::RIGHT));
			cd->addOption(101, "Tell me more about the first phrase.");
			cd->addOption(102, "Tell me more about the second phrase.");
			cd->addOption(103, "Tell me more about the third phrase.");
			cd->addOption(104, "Tell me more about his dragonfire.");
			cd->addOption(79, "I'm ready then. Bring me to the island!");
		}
		else
		{
			ui->addTalkText(new ChatInformation("No.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("No?", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("Yes, I'm not going to tell you anything. I think the challenge of fighting the dragon will be better and more fun if you fought him first without learning all of his mechanics beforehand.", chatNS::RIGHT));
			cd->addOption(79, "I'm ready then. Bring me to the island!");
			if (questData->getValue("minorTaskStatus") == 0) //Not staarted
			{
				cd->addOption(81, "You were saying something about a favour?");
			}
			else
			{
				ui->addTalkText(new ChatInformation("But, as agreed, I'll tell you anything I know about the dragon once you've helped me with that favour.", chatNS::RIGHT));
			}
		}
		cd->addOption(0, "Hold on for now, I'll be back later once I'm ready.");
		ui->addTalkText(cd);
		break;
	case 81: //You needed a favour?'
		ui->addTalkText(new ChatInformation("That's right, I need one small favour.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Oh no. No no no no no no.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("I need you to help me bring", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Did you not hear me? No.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("But...", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("No buts. I know how your \"One Small Favour\" works. It is never small. Never quick. And it will end up with me doing a large amount of favours just becuase of it.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("No, this will be different. It will be a very fast favour - trust me!", chatNS::RIGHT));
		cd->addOption(82, "Very well, what is it?");
		cd->addOption(0, "No means no.");
		ui->addTalkText(cd);
		break;
	case 82: //Very well, what is it?
		gem->informListeners(new GameEvent_EntityAction(ii));
		ui->addTalkText(new ChatInformation("I need you to help me bring this broken net to the fisherman, and ask him to help me fix up the net for him.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Yup. As I predicted. This is not going to be a good quest.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Please! Just bring it to the fisherman, and bring it back here. Simple and fast.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("This is not going to be like that. It'll be a lot worse. The fisherman will have some trouble, and therefore can't fix your net.", chatNS::LEFT));
		cd->addOption(83, "Tell me, why can't you do this yourself?");
		cd->addOption(85, "Why do you need that net fixed anyway?");
		cd->addOption(87, "What's in it for me if I do this extremely long quest?");
		cd->addOption(0, "Sorry, ask someone else.");
		ui->addTalkText(cd);
		break;
	case 83: //Tell me, why can't you do this yourself?
		ui->addTalkText(new ChatInformation("I can't leave this post! My job is to stay here and help ferry people to ideal island to fight the dragon, and warn them about it!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("What if I leave to travel and then someone comes looking to go to ideal island? What then? I have to stay here!", chatNS::RIGHT));
		cd->addOption(84, "How about I stay here and ferry adventurers across for you?");
		cd->addOption(85, "Why do you need that net fixed anyway?");
		cd->addOption(87, "What's in it for me if I do this extremely long quest?");
		cd->addOption(0, "Sorry, ask someone else.");
		ui->addTalkText(cd);
		break;
	case 84: //How about I stay here and ferry adventurers for you?
		questData->setValue("minorTaskAskerBringNet", 1);
		ui->addTalkText(new ChatInformation("In the meantime, you can go and get that net fixed, and return once you're done!", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("That's a nice idea, but no. Do you even know how to sail? Can you ferry adventurers across the river? You don't seem to know anything about sailing!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Furthermore, there's no way I'm leaving my boat with a stranger. What if you just took it and left?", chatNS::RIGHT));
		cd->addOption(85, "Why do you need that net fixed anyway?");
		cd->addOption(87, "What's in it for me if I do this extremely long quest?");
		cd->addOption(0, "Sorry, ask someone else.");
		ui->addTalkText(cd);
		break;
	case 85: //Why do you need that net fixed anyway?
		questData->setValue("minorTaskAskerNeedsNet", 1);
		ui->addTalkText(new ChatInformation("Well, I stay here everyday, just ferrying adventurers across. I get bored!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("One of the things I do to pass the time is fish. I grab my net and fish. But I need my net fixed to fish!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Please help me! The net has been broken for a long time now, it's been really boring.", chatNS::RIGHT));
		cd->addOption(83, "Tell me, why can't you do this yourself?");
		cd->addOption(86, "You can fish here? But I don't see any fishing spots.");
		cd->addOption(87, "What's in it for me if I do this extremely long quest?");
		cd->addOption(0, "Sorry, ask someone else.");
		ui->addTalkText(cd);
		break;
	case 86: //You can fish here? But I don't see any fishing spots.
		ui->addTalkText(new ChatInformation("Ah. I see you know a thing or two about fishing.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Yes, it's true, you typically need fishing spots to fish, but a skilled fisherman like me can fish anywhere as long as there is water!", chatNS::RIGHT));
		cd->addOption(83, "Tell me, why can't you do this yourself?");
		cd->addOption(87, "What's in it for me if I do this extremely long quest?");
		cd->addOption(0, "Sorry, ask someone else.");
		ui->addTalkText(cd);
		break;
	case 87: //What's in it for me if I do this extremely long quest?
		ui->addTalkText(new ChatInformation("Come on now, You're exaggerating it. This quest won't be long, it'll be a quick one.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("No, it certainly won't. I know how these quests work.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Well, if you help me I'll tell you what I know about the dragon below.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Trust me, I know a lot about it from all the adventurers that have passed by and that I've ferried to that island.", chatNS::RIGHT));
		cd->addOption(83, "Tell me, why can't you do this yourself?");
		cd->addOption(85, "Why do you need that net fixed anyway?");
		if (questData->getValue("minorTaskAskerNeedsNet") == 1 && questData->getValue("minorTaskAskerBringNet") == 1)
		{
			cd->addOption(88, "Against my better judgement, I'll help.");
		}
		else
		{
			ui->addTalkText(new ChatInformation("You are still unconvinced that this annoying quest should be done.", chatNS::MIDDLE));
		}
		cd->addOption(0, "Sorry, ask someone else.");
		ui->addTalkText(cd);
		break;
	case 88: //Against my better judgement, I'll help.
		gem->informListeners(new GameEvent_ItemReceived(grpg->getItemLoader()->getItem(39)));
		ui->addTalkText(new ChatInformation("But only because you seem to really need that help.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Thank you! You won't regret this. Here you go, here's the broken net.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("And I'm sure I'll regret saying yes.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("You take the net.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("You can find the fisherman west of the chicken pen.", chatNS::RIGHT));
		cd->addOption(89, "Time to start the most annoying quest ever.");
		ui->addTalkText(cd);
		break;
	case 89: //Time to start the most annoying quest ever.
		delete cd;
		ui->removeWindow();
		entity->sayMessage("Don't worry, it'll be a short quest!");
		break;
	case 90: //About your favour...
		ui->addTalkText(new ChatInformation("Ah yes, how are you getting along?", chatNS::RIGHT));
		if (questData->getValue("minorTaskStatus") == 1)
		{
			ui->addTalkText(new ChatInformation("I haven't got around to doing it yet, actaully.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("Well, please do, I'm getting bored without having fishing to pass the time.", chatNS::RIGHT));
		}
		else if (questData->getValue("minorTaskStatus") == 2)
		{
			ui->addTalkText(new ChatInformation("Well, I just went and talked to the fisherman.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("And...?", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("He asked me to do another favour for him, because he's feeling sick and can't fix the net while he's sick!", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("See? I told you this quest is not going to be simple.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("Please? It's just one more favour.", chatNS::RIGHT));
			cd->addOption(99, "No. I've proven my point.");
			cd->addOption(100, "Very well, I'll continue the quest.");
		}
		else if (questData->getValue("minorTaskStatus") == 3)
		{
			ui->addTalkText(new ChatInformation("The fisherman is sick and cannot help you fix the net.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("However, I've spoke with the doctor and he agrees to try and help the fisherman.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("Awesome! Perhaps you should check up on them and see how they're doing?", chatNS::RIGHT));
		}
		else if (questData->getValue("minorTaskStatus") == 4)
		{
			ui->addTalkText(new ChatInformation("It's done! The net is fixed! Here you go!", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("You hand the net over to the captain.", chatNS::MIDDLE));
			ui->addTalkText(new ChatInformation("Awesome! Now that wasn't too hard, was it?", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("No, no it wasn't as long as I expected it to be.", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("That's good. Do some favours for people once in a while. It won't be annoying or long.", chatNS::RIGHT));
			gem->informListeners(new GameEvent_ItemUsed(grpg->getItemLoader()->getItem(39)));
			cd->addOption(0, "I'll keep that in mind.");
			ui->addTalkText(cd);
			break;
		}
		cd->addOption(79, "Can you ferry me to ideal island?");
		cd->addOption(0, "I'll get right on doing your favour.");
		ui->addTalkText(cd);
		break;
	case 91: //Can you fix this broken net?    ============== Fisherman
		ui->addTalkText(new ChatInformation("Oh yes, I would love to.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("But...", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("My, you sure can read minds. But yes, I'm afraid I'm feeling slightly sick. I'm in no mood to be mending nets right now.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("You sigh.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("Perhaps you could do me a favour?", chatNS::RIGHT));
		cd->addOption(92, "I knew it. It would just lead to another favour.");
		cd->addOption(0, "I don't want to do this quest. Forget I asked.");
		ui->addTalkText(cd);
		break;
	case 92: //I knew it. It would just lead to another favour.
		gem->informListeners(new GameEvent_EntityAction(ii));
		ui->addTalkText(new ChatInformation("All I need you to do is to head to the doctor and ask him if he could help me.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Great. Asking a favour from someone else again.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Come on! It'll be a very fast one, the doctor's not far from here. Won't take you long at all.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("That's what the captain said. And now? You're asking for a favour. This is absolutely NOT going to be a fast quest.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("...But I agreed to help, so I'll see this annoying quest through.", chatNS::LEFT));
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
		// ======================== doctor
	case 93: //Can you heal me?
		ui->addTalkText(new ChatInformation("The doctor heals you.", chatNS::MIDDLE));
		thePlayer->setHealth(thePlayer->getSkills()->at(skillNS::ID_SKILL_TOUGHNESS).getSkillLevel());
		ui->addTalkText(new ChatInformation("For reference, next time you can just right click from me and choose to request healing.", chatNS::RIGHT));
		cd->addOption(0, "I'll keep that in mind.");
		ui->addTalkText(cd);
		break;
	case 94: //How do you heal me like that?
		ui->addTalkText(new ChatInformation("If I told you, I'd have to kill you.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Whaaaa...?", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Let's just say it's a secret.", chatNS::RIGHT));
		cd->addOption(95, "Can you make me a portable healing object?");
		cd->addOption(0, "I'll keep that in mind.");
		ui->addTalkText(cd);
		break;
	case 95: //Can you somehow make an object that will heal me fully if I use it?
		ui->addTalkText(new ChatInformation("I'll pay good money for it", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("No.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("And now you're going to ask why, and the answer is, that's a secret as well. If you want healing, come to me, and I'll heal you for free.", chatNS::RIGHT));
		cd->addOption(96, "Could you follow me everywhere I go to heal me all the time?");
		cd->addOption(0, "Very well.");
		ui->addTalkText(cd);
		break;
	case 96: //Can you follow me everywhere I go to heal me all the time?
		ui->addTalkText(new ChatInformation("Your requests are getting more ridiculous!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Adventurer, you are not the only person in this world. Get that into your big, fat head.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I have other people to care for.", chatNS::RIGHT));
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 97: //The fisherman says he needs some help.
		{
			gem->informListeners(new GameEvent_EntityAction(ii));
			ui->addTalkText(new ChatInformation("He does? Very well, I'll go take a look at him them.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("The Doctor leaves for the fisherman.", chatNS::MIDDLE));
			VECTOR2 newDestination = grpg->getMapLoader()->translateIdToCoords('-');
			newDestination.x += tileNS::WIDTH;
			entity->setDestination(new Point(newDestination));
			cd->addOption(0, "Exit chat");
			ui->addTalkText(cd);
			break;
		}
	case 98: //How are you feeling now> =============== Fisherman
		{
			gem->informListeners(new GameEvent_EntityAction(ii));
			ui->addTalkText(new ChatInformation("Much better. Thank you.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("So... no more favours?", chatNS::LEFT));
			ui->addTalkText(new ChatInformation("Yes, that's right. I can fix the net now.", chatNS::RIGHT));
			ui->addTalkText(new ChatInformation("The fisherman quickly takes your net and repairs it.", chatNS::MIDDLE));
			ui->addTalkText(new ChatInformation("There you go, good as new.", chatNS::RIGHT));
			//Move doctor back to position
			VECTOR2 doctorStart = grpg->getMapLoader()->translateIdToCoords('+');
			stringstream ss;
			ss << doctorStart.x << "," << doctorStart.y;
			Entity* doctor = grpg->getSpawnLink(ss.str());
			doctor->setDestination(new Point(doctorStart));
			cd->addOption(0, "Wow, thanks");
			ui->addTalkText(cd);
			break;
		}
	case 99: //Back to Captain Point of No Return - see case 90 - No, I've proven my point.
		gem->informListeners(new GameEvent_EntityAction(ii));
		ui->addTalkText(new ChatInformation("Everyone I ask is just going to ask me to do one additional favour for them. ", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("Oh, come on. Please? I get really bored without my net.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("I'm sure it'll just end at one additional favour... it won't be too long!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("No, it won't. Everyone will just ask for another favour.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("How about this. If you help me complete this quest now, I'll teach you some tips I learned about fishing over the time I spent here.", chatNS::RIGHT));
		cd->addOption(0, "I'll think about it");
		ui->addTalkText(cd);
		break;
	case 100: //Very well, I'll continue the quest
		ui->addTalkText(new ChatInformation("Thank you.", chatNS::RIGHT));
		cd->addOption(79, "Can you ferry me to ideal island?");
		cd->addOption(0, "I'll get right on doing your favour.");
		ui->addTalkText(cd);
		break;
	case 101: //Tell me more about the first phrase
		ui->addTalkText(new ChatInformation("The first phrase will be how you will fight Aildiuln once you engage him", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("In this phrase, he will have no special mechanics except for simply breathing fire.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Many say that this phrase is the shortest, and will change quickly, so don't expect it to stay that easy.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("A lot of people say it is best if you do as much damage to him as possible during the short amount of time it lasts.", chatNS::RIGHT));
		cd->addOption(105, "Thank you for that information");
		ui->addTalkText(cd);
		break;
	case 102: //Tell me more about the second phrase
		ui->addTalkText(new ChatInformation("Ah, the second phrase. Everyone claims that once this starts, Aildiuln will take to the skies, using his wings to fly up!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("He will then continue to breathe dragonfire down at you!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Some people have say that he has managed to trap them into a corner, and breathe dragonfire right at you, causing you to be unable to dodge them", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Few say they managed to see some eggs when he flew! Imagine! A nest of dragon eggs!", chatNS::RIGHT));
		cd->addOption(105, "Thank you for that information");
		ui->addTalkText(cd);
		break;
	case 103: //Tell me more about the third phrase
		ui->addTalkText(new ChatInformation("Ah, not many have reached to this phrase.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Those that have disagree on how to reach this phrase. Some say you need to insult Aildiuln enough so he comes down to hit you. Others say you need to hide until he cannot see you from above.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Many, however, agree that when this phrase is reached, a group of hidden dragons appear to come out and fight you, on Aildiuln's side!", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Unfortunately, that's all I've heard about it.", chatNS::RIGHT));
		cd->addOption(105, "Thank you for that information");
		ui->addTalkText(cd);
		break;
	case 104: //Tell me more about the magical dragonfire
		ui->addTalkText(new ChatInformation("Aildiuln is a dragon with the ability to breathe magical dragonfire.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Everyone who has been hit by it agree that their armour was completely useless to protecting them", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("Some, however, claim that they managed to forge a special shield that managed to protect themselves from the dragonfire.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("One person even said that he managed to create a potion that would help his body defend against it! I doubt that's true though, seems very unlikely.", chatNS::RIGHT));
		cd->addOption(105, "Thank you for that information");
		ui->addTalkText(cd);
		break;
	case 105: //Thank you for that information
		ui->addTalkText(new ChatInformation("You're welcome. Is there anything else you would like to know?", chatNS::RIGHT));
		cd->addOption(101, "Tell me more about the first phrase.");
		cd->addOption(102, "Tell me more about the second phrase.");
		cd->addOption(103, "Tell me more about the third phrase.");
		cd->addOption(104, "Tell me more about his dragonfire.");
		cd->addOption(79, "Nope, I'm ready now. Bring me to the island!");
		cd->addOption(0, "No, thanks for everything!");
		ui->addTalkText(cd);
		break;
	case 106: //Doctor - what's up with that shiny crystal?
		ui->addTalkText(new ChatInformation("What shiny crystal?", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("That one, inside this house.", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("I don't see any shiny crystal in here. It's just me and you. Are you trying to trick me or something?", chatNS::RIGHT));
		cd->addOption(107, "I believe you.");
		cd->addOption(108, "You're lying");
		ui->addTalkText(cd);
		break;
	case 107: // I believe you about the save crystal
		questData->setValue("saveCrystalDoctorAsked", 1);
		ui->addTalkText(new ChatInformation("Is there something wrong wtih you? Maybe you need some healing. I can do that.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("No thanks, I'm good.", chatNS::LEFT));
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 108: //I think you're lying about the crystal
		questData->setValue("saveCrystalDoctorAsked", 2);
		ui->addTalkText(new ChatInformation("Come on, you have to see the crystal - it's right there!", chatNS::LEFT));
		ui->addTalkText(new ChatInformation("I've told you, there is no crystal in this house. Now leave me be, joker! I have important work to attend to, and people who need my help.", chatNS::RIGHT));
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 109: // Lawyer - tell me more about the images
		ui->addTalkText(new ChatInformation("The images for the tabs were found from iconfinder, as well as the up arrow used during a skill level up.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("All other images are either created by the developers themselves, or found early during development and no credit was required, so their source was not recorded.", chatNS::RIGHT));
		cd->addOption(110, "Tell me about the sound");
		cd->addOption(111, "Tell me about the music");
		cd->addOption(0, "Nothing else, thanks.");
		ui->addTalkText(cd);
		break;
	case 110: //Tell me more about the sound
		ui->addTalkText(new ChatInformation("The sword attack used by the player was from http://www.soundsnap.com/tags/sword, 4th item on the list called Sword Schwing", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The skil level up effect sound was from http://opengameart.org/content/level-up-sound-effects, licensed under CC by 3.0", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("And that's all, no other sounds need to be credited. The rest were probably created using bfxr, a sound generating tool for developers.", chatNS::RIGHT));
		cd->addOption(109, "Tell me about the images");
		cd->addOption(111, "Tell me about the music");
		cd->addOption(0, "Nothing else, thanks.");
		ui->addTalkText(cd);
		break;
	case 111: //Tell me more about the music
		ui->addTalkText(new ChatInformation("The following pieces of music are used in the game. They may have been spliced to fit different parts of the game. All music can be found at incompetech.com, by Kevin MacLeod. They are licensed under Creative Commons: By Attribution 3.0.", chatNS::RIGHT));
		ui->addTalkText(new ChatInformation("The music tracks are: Easy Lemon, Movement Proposition, Volatile Reaction and Five Armies", chatNS::RIGHT));
		cd->addOption(109, "Tell me about the images");
		cd->addOption(110, "Tell me about the sound");
		cd->addOption(0, "Nothing else, thanks.");
		ui->addTalkText(cd);
		break;
	default:
		stringstream ss;
		ss << "Warning: Unknown ChatData ID: " << co.id;
		ui->addChatText(ss.str());
		//Also doesn't use cd
		delete cd;
		break;
	}
}

void TalkBehavior::generateEasterQuestTalk(ChatDecision* cd)
{
	cd->addOption(7, "Let's talk about the feathers you need.");
	cd->addOption(10, "Let's talk about the easter egg that you need.");
	cd->addOption(13, "Let's talk about feeding the bird");
	cd->addOption(0, "Tasks to do, gotta go");
}
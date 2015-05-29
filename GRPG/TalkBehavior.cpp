//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "talkBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "grpg.h"
#include "ShriveledMan.h"

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
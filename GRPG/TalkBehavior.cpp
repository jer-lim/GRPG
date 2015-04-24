//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "talkBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "grpg.h"

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
					if (!questData->getValue("easterStarted"))
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
		ui->addTalkText(new ChatInformation("Of course.", chatNS::RIGHT));
		generateEasterQuestTalk(cd);
		ui->addTalkText(cd);
		break;
	case 7: //Let's talk about the feathers you need
		ui->addTalkText(new ChatInformation("Sure, what do you want to know?", chatNS::RIGHT));
		cd->addOption(8, "What do you need these feathers for?");
		if (questData->getValue("easterFeatherRequired"))
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
		if (questData->getValue("easterFeatherRequired"))
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
		if (questData->getValue("easterFeatherRequired"))
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
		if (!questData->getValue("easterEggRequired"))
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
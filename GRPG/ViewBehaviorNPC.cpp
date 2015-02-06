//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "viewbehaviorNPC.h"
#include "NPC.h"
#include "UI.h"

string ViewBehaviorNPC::displayText(){ return "Examine " + ii->getname(); }
void ViewBehaviorNPC::action(){
	//Type stuff into the chat to say the description
	ui->addChatText(ii->getdescription());
}
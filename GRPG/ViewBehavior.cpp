//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "viewbehavior.h"
#include "NPC.h"
#include "UI.h"

string ViewBehavior::displayText(){ return "Examine " + name; }
void ViewBehavior::action(){
	//Type stuff into the chat to say the description
	ui->addChatText(examineText);
}
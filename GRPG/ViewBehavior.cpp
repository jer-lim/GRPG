#include "viewbehavior.h"
#include "NPC.h"
#include "UI.h"

string ViewBehavior::displayText(){ return "Examine " + name; }
void ViewBehavior::action(){
	//Type stuff into the chat to say the description
	ui->addChatText(examineText);
}
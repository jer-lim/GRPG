#include "viewbehaviorNPC.h"
#include "NPC.h"

string ViewBehaviorNPC::displayText(){ return "Examine " + ii->getname(); }
void ViewBehaviorNPC::action(){
	//Type stuff into the chat to say the description
	//ii->getdescription();
}
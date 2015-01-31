#include "viewBehaviorItem.h"
#include "NPC.h"
#include "UI.h"

void ViewBehaviorItem::action(){
	//Type stuff into the chat to say the description
	ui->addChatText(ii->getItem()->getDescription());
}
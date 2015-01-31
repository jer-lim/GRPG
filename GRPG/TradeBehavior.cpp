#include "tradeBehavior.h"
#include "NPC.h"
#include "UI.h"

string TradeBehavior::displayText(){ return "Trade with " + ii->getname(); }
void TradeBehavior::action(){
	ui->drawWindow("Grocer's Store");
}
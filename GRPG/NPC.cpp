#include "NPC.h"
#include "PersonLoader.h"
#include "drawManager.h"

Entity* NPC::spawn(Game* gamePtr, int npcId, VECTOR2 coords, Entity* victim){
	Entity* enemy = new Entity();
	enemy->initialize(gamePtr, gamePtr->getPersonLoader()->getNPC(npcId));
	enemy->setX(coords.x);
	enemy->setY(coords.y);
	enemy->setVictim(victim);
	gamePtr->getDrawManager()->addObject(enemy, 1);
	return enemy;
}

void NPC::setTalkText(string txt) {
	stringstream ss;
	char tempChar;
	queue<char> last2Characters;
	string realText = "";
	ss << txt;
	tempChar = ss.get();
	while (tempChar != EOF){
		last2Characters.push(tempChar);
		if (last2Characters.size() > 2){
			realText += last2Characters.front();
			last2Characters.pop();
		}
		if (last2Characters.front() == '\\' && tempChar == 'n'){
			realText = realText + '\n';
			last2Characters.pop();
			last2Characters.pop();
		}
		tempChar = ss.get();
	}

	while (last2Characters.size() > 0){
		realText += last2Characters.front();
		last2Characters.pop();
	}


	talkText = realText;
}
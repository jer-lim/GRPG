//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "Quest.h"
#include "GameEventManager.h"

Quest::Quest(GameEventManager* qcM, QuestData* qd, string nama, string descript, int gp, Button* b)
{
	name = nama;
	description = descript;
	gold = gp;
	ui_element = b;
	qcM->addListener(this);
	questData = qd;
	//reward = prize;
}
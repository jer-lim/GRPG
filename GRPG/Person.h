//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _PERSON_H                 // Prevent multiple definitions if this 
#define _PERSON_H				// file is included in more than one place

#include "constants.h"
#include <string>
#include "game.h"
#include "textureManager.h"

namespace PersonNS{
	const std::string spriteDirectory = "assets/npcs/sprites/";

	//IDs
	const int ID_NPC_DOCTOR = 0;
	const int ID_NPC_GROCER = 1;
	const int ID_NPC_BLACKSMITH = 2;
		
	const int ID_NPC_SKELETON = 3;
	const int ID_NPC_SKELETON_KING = 4;
	const int ID_NPC_GOBLIN = 5;
	const int ID_NPC_GOBLIN_CHIEF = 6;
	const int ID_NPC_UNDEAD = 7;
	const int ID_NPC_UNDEAD_CMDR = 8;
	const int ID_NPC_OGRE = 9;
	const int ID_NPC_OGRE_BOSS = 10;
	const int ID_NPC_GOLEM = 11;
	const int ID_NPC_GOLEM_GUARDIAN = 12;
	const int ID_NPC_DRAGON = 13;
	const int ID_NPC_AIDIL = 14;
}

class Person
{
private:
	string imgFileName;
	int instanceCount = 0;//Keep track for textureManager
	TextureManager* textureManager = nullptr;
	float movementSpeed;
	float attackCooldown;

	float height;
	float width;
	int numOfCols;

	float colliHeight;
	float colliWidth;
public:
	//Static characters used to refer to them (Similar to skills)
	static Person* thePlayer;

	Person(){}
	void destroy(){ SAFE_DELETE(textureManager); }
	virtual ~Person(){
		destroy();
	}
	Person(string i, float mov, float atk, float h, float w, int cols, float colHeight, float colWidth)
	{
		imgFileName = i;
		movementSpeed = mov;
		attackCooldown = atk;

		height = h;
		width = w;
		numOfCols = cols;

		colliHeight = colHeight;
		colliWidth = colWidth;
	}

	string getImgFileName() { return imgFileName; }
	void setInstanceCount(int i){ 
		instanceCount = i;
		if (instanceCount <= 0)
		{
			SAFE_DELETE(textureManager);
		}
	}
	int getInstanceCount(){ return instanceCount; }
	TextureManager* initializeTexture(Game* gamePtr){
		if (instanceCount <= 0)
		{
			textureManager = new TextureManager();
			if (!textureManager->initialize(gamePtr->getGraphics(), (PersonNS::spriteDirectory + imgFileName).c_str()))
			{
				throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize person texture " + imgFileName));
			}
		}
		instanceCount++;
		return textureManager;
	}
	float getMovementSpeed() { return movementSpeed; }
	float getAttackCooldown() { return attackCooldown; }
	float getHeight() { return height; }
	float getWidth() { return width; }
	int getNumOfCols() { return numOfCols; }

	float getColliHeight() { return colliHeight; }
	float getColliWidth() { return colliWidth; }

	void Release() {
		setInstanceCount(instanceCount - 1);
	}

	virtual string getType(){ return "PERSON"; }
};

#endif
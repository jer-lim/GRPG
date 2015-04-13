//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _PLAYER_H                 // Prevent multiple definitions if this 
#define _PLAYER_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "playerSkill.h"
#include "List.h"
#include "Inventory.h"
#include <map>
#include "textDX.h"
#include "Resource.h"
#include "Armor.h"
#include "Weapon.h"

using namespace std;

namespace playerNS
{
	const int WIDTH = 64;                   // image width
	const int HEIGHT = 64;                  // image height
	const int X = GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
    const float SPEED = 100;                // 100 pixels per second
    const int   TEXTURE_COLS = 8;           // texture has 8 columns

	const float textTimeDisplay = 3;

	const float startingRegnerationTime = 60;
	const float fishingWaitTime = 10;
	const float miningWaitTime = 10;
}

class Grpg;

// The main player of the game. The hero that will kill Aidleliun or however you spell that fking name
class Player : public Entity
{
private:
	map <int, PlayerSkill> skills;

	Grpg* game;

	//Talking variables
	std::string textMessage;
	TextDX* fontToUse;
	float timeLeft;
	VECTOR2 textSize;

	float regenerationDelay;

	// The current action of the player, if skilling
	// Refers to either of the values in Resource
	int currentAction;
	// For cooking
	bool nearStove;
	// How long before the player gets the next skill item
	float actionDelay;	
	// The resource image that is drawn above hte player
	Image fishingImage;
	TextureManager* tm;
	Image miningImage;
	TextureManager* mm;
protected:
	void restartCounter(int startingTime, int skilLevel);

public:
    // constructor
    Player();
	~Player(){
		for (map<int, PlayerSkill>::iterator it = skills.begin(); it != skills.end(); ++it){
			it->second.destroy();
		}
		skills.clear();
		SAFE_DELETE(tm);
		SAFE_DELETE(mm);
		game = NULL;
	}

	// Get functions

	// Get the player's skills, which is a map of <int, PlayerSkill>.
	// All integers can be retrieved with Skill:ID_
	map<int, PlayerSkill>* getSkills() { return &skills; }

	bool getNearStove() { return nearStove; }
	void setNearStove(bool r) { nearStove = r; }

	// Specific player functions

	// Causes the player message to appear right above the player for a period of time,
	// using the specified font
	virtual void sayMessage(std::string message, TextDX* font);

    // inherited member functions
	virtual void draw(Viewport* viewport);
    virtual bool initialize(Game *gamePtr);
	virtual void update(float frameTime, Game* gamePtr);

	// This entity has been damaged by another entity
	// Pass in the other entity's attack and strength.
	// Returns the amount of damage dealt.
	virtual int damage(int atk, int str);

	// Displays the text that will be shown when the mouse is over it
	// Overrides the Entity function so that nothing is shown on mouse over the player
	virtual string view() { return ""; }

	// Return whether the entity is an attackable enemy
	// Overrides the Entity function so that the player is always marked as an ally
	virtual bool isEnemy() { return false; }

	// Sets the health of the player
	// If the player's health is more than his max health, sets it to the max health
	virtual void setHealth(int h)
	{
		health = h;
		if (health > skills[skillNS::ID_SKILL_TOUGHNESS].getSkillLevel())
		{
			health = skills[skillNS::ID_SKILL_TOUGHNESS].getSkillLevel();
		}
	}

	//Returns the combat level of the player
	//Calculated by 1/4*(13/10*(ATK + STR + DEF + TOUGH))
	virtual int calculateCombatLevel()
	{
		return 0.25*(1.3*
			(skills[skillNS::ID_SKILL_ATTACK].getSkillLevel() + 
			skills[skillNS::ID_SKILL_STRENGTH].getSkillLevel() + 
			skills[skillNS::ID_SKILL_DEFENSE].getSkillLevel() + 
			skills[skillNS::ID_SKILL_TOUGHNESS].getSkillLevel()));
	}

	//Starts the player fishing
	//flip defines if the player's avatar should be flipped horizontally
	//If true, player is facing left, otherwise facing right
	virtual void startFishing(bool flip);

	//Starts the player mining
	//flip defines if the player's avatar should be flipped horizontally
	//If true, player is facing left, otherwise facing right
	virtual void startMining(bool flip);

	virtual float getDamageReduction(){
		if (getInventory()->getSlotBody() == nullptr)
			return 1;
		else
		{
			float dmgReduction = ((Armor*)getInventory()->getSlotBody()->getInventoryItem()->getItem())->getdmgReduction();
			return dmgReduction;
		}
	}

	virtual float getDefenceMultiplier(){
		if (getInventory()->getSlotBody() == nullptr)
			return 1;
		else
		{
			float defMultiplier = ((InventoryEquipment*)getInventory()->getSlotBody()->getInventoryItem())->getSmithingMaterial()->getDefMultiplier();
			defMultiplier += ((Armor*)getInventory()->getSlotBody()->getInventoryItem()->getItem())->getdefMultiplier();
			return defMultiplier;
		}
	}

	virtual float getAttackSpeedReduction(){
		if (getInventory()->getSlotHand() == nullptr)
			return 1;
		else
		{
			float attackSpeed = ((InventoryEquipment*)getInventory()->getSlotHand()->getInventoryItem())->getSmithingMaterial()->getSpdMultiplier();
			attackSpeed += ((Weapon*)getInventory()->getSlotBody()->getInventoryItem()->getItem())->getspdMultiplier();
			return attackSpeed;
		}
	}

	virtual float getDamageMultiplier(){
		if (getInventory()->getSlotHand() == nullptr)
			return 1;
		else
		{
			float dmgMultiplier = ((InventoryEquipment*)getInventory()->getSlotHand()->getInventoryItem())->getSmithingMaterial()->getStrMultiplier();
			dmgMultiplier += ((Weapon*)getInventory()->getSlotBody()->getInventoryItem()->getItem())->getstrMultiplier();
			return dmgMultiplier;
		}
	}

	virtual string getType(){ return "PLAYER"; }

	UI* getUI();
};
#endif


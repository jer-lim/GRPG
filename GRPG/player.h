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
#include "Rift.h"

using namespace std;

namespace playerNS
{
	const string hurtPhraseFileName = "assets/misc/hurt_phrases.gdef";

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
	const float thievinWaitTime = 3;
	const float dragonfireImmuneTime = 1;
	const float combatLeaveDelay = 3;
}

class Grpg;

// The main player of the game. The hero that will kill Aidleliun or however you spell that fking name
class Player : public Entity
{
private:
	map <int, PlayerSkill> skills;

	Grpg* game;

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
	vector<string> hurtPhrases;

	//Thieving stun
	float thievingCooldown;
	//Is the player immune to dragonfire? If so, for how long?
	//If <0 = not immune
	//>0 = immune for that amount of time.
	float dragonfireImmunity;

	//Marks if the player is teleporting or not.
	//If teleporting, draw manager will not bother to check mouseOverEntity and set it to nullptr automatically.
	//Failure to do so may result in a crash during teleportation.
	//Should be set to 2 when required. This is not a boolean as when a teleportation is done,
	//the mapLoader load code is still ran ONCE before the maploader actually shifts the map;
	//resulting in isTeleporting set to false on that run. Thus now it's an int set to 2, 
	//and whenever teleporting is not detected it reduces it by 1.
	int isTeleporting;

	//A reference to the rift portal that brought the player here.
	//If player is not in a rift, this will point to nullptr.
	Rift* riftPortal;

	int totalDeaths;
	int aidilKills;
	//Tracks the player's best time taken to kill Aidil, measured in seconds
	//if 0, then means no time has been recordded.
	float aidilKillTime;
	bool inCombat;
	//delay before leaving combat
	float combatLeaveDelay;
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
	virtual bool inDarkRealm() { return riftPortal != nullptr; }
	virtual void setRiftPortal(Rift* e)
	{
		riftPortal = e;
		Entity::setIsInDarkRealm(inDarkRealm());
	}

	virtual Rift* getRiftPortal() { return riftPortal; }

	// Specific player functions

    // inherited member functions
	virtual void draw(Viewport* viewport);
    virtual bool initialize(Game *gamePtr);
	virtual void update(float frameTime, Game* gamePtr);

	// This entity has been damaged by another entity
	// Pass in the other entity's attack and strength.
	// Returns the amount of damage dealt.
	virtual int damage(int atk, int str);

	// damage
	// This entity has been damaged by another entity, taking non-negatable damage
	// Pass in the damage dealt
	virtual void damage(int damageTaken);

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

	//Starts the thieving cooldown
	//The player cannot perform any action during this time
	virtual void failThieving() { thievingCooldown = playerNS::thievinWaitTime; }

	//Returns true if the player is currently stunned due to a failed thieve, false otherwise
	virtual bool hasFailedThieve() { return thievingCooldown > 0; }

	//Sets the player as being hit by dragonfire, resetting dragonfireImmunity.
	virtual void hitByDragonfire() {
		dragonfireImmunity = playerNS::dragonfireImmuneTime;
	}

	virtual bool getImmunityToDragonfire() {
		return dragonfireImmunity > 0;
	}

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
		float defMultiplier = 1;
		if ((InventoryEquipment*)getInventory()->getSlotBody() != nullptr)
		{
			defMultiplier = ((InventoryEquipment*)getInventory()->getSlotBody()->getInventoryItem())->getSmithingMaterial()->getDefMultiplier();
		}
		if ((Armor*)getInventory()->getSlotBody() != nullptr)
		{
			defMultiplier += ((Armor*)getInventory()->getSlotBody()->getInventoryItem()->getItem())->getdefMultiplier();
		}
		return defMultiplier;
	}

	virtual float getAttackSpeedReduction(){
		float attackSpeed = 1;
		if ((InventoryEquipment*)getInventory()->getSlotHand() != nullptr)
		{
			attackSpeed = ((InventoryEquipment*)getInventory()->getSlotHand()->getInventoryItem())->getSmithingMaterial()->getSpdMultiplier();
		}
		if ((Weapon*)getInventory()->getSlotBody() != nullptr)
		{
			attackSpeed += ((Weapon*)getInventory()->getSlotBody()->getInventoryItem()->getItem())->getspdMultiplier();
		}
		return attackSpeed;
	}

	virtual float getDamageMultiplier(){
		float dmgMultiplier = 1;
		if ((InventoryEquipment*)getInventory()->getSlotHand() != nullptr)
		{
			dmgMultiplier = ((InventoryEquipment*)getInventory()->getSlotHand()->getInventoryItem())->getSmithingMaterial()->getStrMultiplier();
		}
		if ((Weapon*)getInventory()->getSlotBody() != nullptr)
		{
			dmgMultiplier += ((Weapon*)getInventory()->getSlotBody()->getInventoryItem()->getItem())->getstrMultiplier();
		}
		return dmgMultiplier;
	}

	virtual string getType(){ return "PLAYER"; }

	virtual int getIsTeleporting() { return isTeleporting; }
	//Set to 2 if a teleport action is performed to prevent crashes with the game's mouse over entity.
	virtual void setIsTeleporting(int t) { isTeleporting = t; }

	UI* getUI();

	virtual void sayRandomHurtPhrase()
	{
		sayMessage(hurtPhrases[rand() % hurtPhrases.size()]);
	}

	virtual int getTotalDeaths() { return totalDeaths; }
	virtual void setTotalDeaths(int t) { totalDeaths = t; }

	virtual int getAidilKills() { return aidilKills; }
	virtual void setAidilKills(int a) { aidilKills = a; }

	virtual float getAidilKillTime() { return aidilKillTime; }
	virtual void setAidilKillTime(float ak) { aidilKillTime = ak; }

	virtual void setInCombat(bool i) { inCombat = i; combatLeaveDelay = playerNS::combatLeaveDelay; }

	virtual map<string, int> getSkillsToSave();

	virtual void loadSkills(map<string, int> skillsData);
};
#endif


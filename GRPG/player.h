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
	
public:
    // constructor
    Player();
	~Player(){
		for (map<int, PlayerSkill>::iterator it = skills.begin(); it != skills.end(); ++it){
			it->second.destroy();
		}
		skills.clear();
		game = NULL;
	}

	// Get functions

	// Get the player's skills, which is a map of <int, PlayerSkill>.
	// All integers can be retrieved with Skill:ID_
	map<int, PlayerSkill>* getSkills() { return &skills; }

	// Specific player functions

	// Causes the player message to appear right above the player for a period of time,
	// using the specified font
	virtual void sayMessage(std::string message, TextDX* font);

    // inherited member functions
	virtual void draw(Viewport* viewport);
    virtual bool initialize(Game *gamePtr);
	virtual void Player::update(float frameTime, Game* gamePtr);
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
};
#endif


#ifndef _PLAYER_H                 // Prevent multiple definitions if this 
#define _PLAYER_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "playerSkill.h"
#include "List.h"
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
	
public:
    // constructor
    Player();

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
    virtual void update(float frameTime);
    virtual void damage(int weapon);
};
#endif


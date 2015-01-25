#ifndef _PLAYER_H                 // Prevent multiple definitions if this 
#define _PLAYER_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "player.h"
#include "playerSkill.h"

namespace playerNS
{
	const int WIDTH = 64;                   // image width
	const int HEIGHT = 64;                  // image height
	const int X = GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
    const float SPEED = 100;                // 100 pixels per second
    const int   TEXTURE_COLS = 8;           // texture has 8 columns
    const int   START_FRAME = 0;      // ship1 starts at frame 0
    const int   END_FRAME = 3;        // ship1 animation frames 0,1,2,3
    const float ANIMATION_DELAY = 0.2f;    // time between frames
}

// inherits from Entity class
class Player : public Entity
{
private:
	PlayerSkill attack;
	PlayerSkill strength;
	PlayerSkill defence;
	PlayerSkill fishing;
	PlayerSkill cooking;
	PlayerSkill mining;
	PlayerSkill smithing;
public:
    // constructor
    Player();

    // inherited member functions
    virtual void draw();
    virtual bool initialize(Game *gamePtr);
    void update(float frameTime);
    void damage(WEAPON);
};
#endif


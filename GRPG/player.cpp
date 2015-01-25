#include "player.h"

//=============================================================================
// default constructor
//=============================================================================
Player::Player() : Entity()
{
    image.spriteData.width = playerNS::WIDTH;           // size of player
	image.spriteData.height = playerNS::HEIGHT;
	//image.spriteData.x = playerNS::X;                   // location on screen
	//image.spriteData.y = playerNS::Y;
	image.spriteData.rect.bottom = playerNS::HEIGHT;    // rectangle to select parts of an image
	image.spriteData.rect.right = playerNS::WIDTH;
	image.setFrameDelay(playerNS::ANIMATION_DELAY);
	image.setFrames(playerNS::START_FRAME, playerNS::END_FRAME);     // set animation frames
	image.setCurrentFrame(playerNS::START_FRAME);
    radius = playerNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;

	attack = PlayerSkill(this, Skill::ATTACK);
	strength = PlayerSkill(this, Skill::STRENGTH);
	defense = PlayerSkill(this, Skill::DEFENSE);
	toughness = PlayerSkill(this, Skill::TOUGHNESS);
	fishing = PlayerSkill(this, Skill::FISHING);
	cooking = PlayerSkill(this, Skill::COOKING);
	mining = PlayerSkill(this, Skill::MINING);
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Player::initialize(Game *gamePtr)
{
    return(Entity::initialize(gamePtr, playerNS::WIDTH, playerNS::HEIGHT, playerNS::TEXTURE_COLS, TEXTURES_IMAGE));
}

//=============================================================================
// draw the player
//=============================================================================
void Player::draw()
{
	Entity::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Player::update(float frameTime)
{
	Entity::update(frameTime);
}

//=============================================================================
// damage
//=============================================================================
void Player::damage(WEAPON weapon)
{
}


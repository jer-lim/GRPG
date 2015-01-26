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

	//skills
	skills[skillNS::ID_SKILL_ATTACK] = PlayerSkill(this, Skill::ATTACK);
	skills[skillNS::ID_SKILL_STRENGTH] = PlayerSkill(this, Skill::STRENGTH);
	skills[skillNS::ID_SKILL_DEFENSE] = PlayerSkill(this, Skill::DEFENSE);
	skills[skillNS::ID_SKILL_TOUGHNESS] = PlayerSkill(this, Skill::TOUGHNESS);
	skills[skillNS::ID_SKILL_FISHING] = PlayerSkill(this, Skill::FISHING);
	skills[skillNS::ID_SKILL_COOKING] = PlayerSkill(this, Skill::COOKING);
	skills[skillNS::ID_SKILL_MINING] = PlayerSkill(this, Skill::MINING);
}
//=============================================================================
// sayMessage
// Causes the message to appear right above the player, using the specified font
//=============================================================================
void Player::sayMessage(std::string message, TextDX* font)
{
	textMessage = message;
	fontToUse = font;
	timeLeft = playerNS::textTimeDisplay;
	// Calculate the text side
	RECT* textRect = new RECT();
	//Don't make the text visible
	textRect->left = -500;
	textRect->top = -500;
	
	font->print(textMessage, *textRect, DT_CALCRECT);
	textSize.x = textRect->right - textRect->left;
	textSize.y = textRect->bottom - textRect->top;
}

//=============================================================================
// Initialize the Player.
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

	//Draw the text right above it
	if (timeLeft > 0)
	{
		//Save the old font colour, and print in black
		DWORD oldColor = fontToUse->getFontColor();
		fontToUse->setFontColor(graphicsNS::BLACK);

		fontToUse->print(textMessage, 
			getX() - textSize.x/2,		//Make text center on top of player
			getY() - playerNS::HEIGHT / 2);

		fontToUse->setFontColor(oldColor);
	}
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Player::update(float frameTime)
{
	Entity::update(frameTime);

	timeLeft -= frameTime;
}

//=============================================================================
// damage
//=============================================================================
void Player::damage(WEAPON weapon)
{
}


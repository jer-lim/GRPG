#include "player.h"
#include "grpg.h"

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
    radius = playerNS::WIDTH/2.0;
    collisionType = entityNS::BOX;
	health = 10;
	regenerationDelay = playerNS::startingRegnerationTime;

	//skills
	skills[skillNS::ID_SKILL_ATTACK] = PlayerSkill(this, Skill::ATTACK);
	skills[skillNS::ID_SKILL_STRENGTH] = PlayerSkill(this, Skill::STRENGTH);
	skills[skillNS::ID_SKILL_DEFENSE] = PlayerSkill(this, Skill::DEFENSE);
	skills[skillNS::ID_SKILL_TOUGHNESS] = PlayerSkill(this, Skill::TOUGHNESS);
	skills[skillNS::ID_SKILL_FISHING] = PlayerSkill(this, Skill::FISHING);
	skills[skillNS::ID_SKILL_COOKING] = PlayerSkill(this, Skill::COOKING);
	skills[skillNS::ID_SKILL_MINING] = PlayerSkill(this, Skill::MINING);
	
	//Start off toughness at a good level
	skills[skillNS::ID_SKILL_TOUGHNESS].gainXP(Skill::calculateXPRequired(11));
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
	textRect->left = 0;
	textRect->top = 0;
	//Note: DT_CALCRECT only sets the rectangle size but does not end up actually drawing the text
	font->print(textMessage, *textRect, DT_CALCRECT);
	textSize.x = textRect->right;
	textSize.y = textRect->bottom;
	delete textRect;
	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd162498%28v=vs.85%29.aspx
}

//=============================================================================
// Initialize the Player.
// Post: returns true if successful, false if failed
//=============================================================================
bool Player::initialize(Game *gamePtr)
{
	game = (Grpg*) gamePtr;
    return(Entity::initialize(gamePtr, Person::thePlayer));
}

//=============================================================================
// draw the player
//=============================================================================
void Player::draw(Viewport* viewport)
{
	Entity::draw(viewport);

	//Draw the text right above it
	if (timeLeft > 0)
	{
		VECTOR2 vpCoords = viewport->translate(getX(), getY());

		//Save the old font colour, and print in black
		DWORD oldColor = fontToUse->getFontColor();
		fontToUse->setFontColor(graphicsNS::BLACK);

		fontToUse->print(textMessage, 
			vpCoords.x - textSize.x/2,		//Make text center on top of player
			vpCoords.y - playerNS::HEIGHT / 2);

		fontToUse->setFontColor(oldColor);
	}
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Player::update(float frameTime, Game* gamePtr)
{
	regenerationDelay -= frameTime;
	if (regenerationDelay <= 0)
	{
		if (health < skills[skillNS::ID_SKILL_TOUGHNESS].getSkillLevel())
			health++;
		regenerationDelay += playerNS::startingRegnerationTime;
	}

	Entity::update(frameTime, gamePtr);

	timeLeft -= frameTime;
}
//=============================================================================
// damage
// This entity has been damaged by another entity
// Pass in the other entity's attack and strength.
// Returns the amount of damage dealt.
//=============================================================================
int Player::damage(int atk, int str)
{
	takeDamage(atk, str, skills[skillNS::ID_SKILL_DEFENSE].getSkillLevel());
	if (health <= 0)
	{
		game->getUI()->addChatText("You died!");
	}
	return damageTaken;
}

//=============================================================================
//Starts the player fishing
//flip defines if the player's avatar should be flipped horizontally
//If true, player is facing left, otherwise facing right
//=============================================================================
void Player::startFishing(bool flip)
{
	image.flipHorizontal(flip);

	InventoryItem* fish = new InventoryItem(game->getItemLoader()->getItem(5), 9);
	Entity* e = new Entity();
	e->initialize(game, fish, true);//anchored if its an inventory
	//y->initialize(this, true);
	inventory.addEntityInventoryItem(e);
}